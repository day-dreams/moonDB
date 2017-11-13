#include "server/worker.h"
#include "logging.h"
#include "translater.h"
#include "vm/vm.h"
#include <chrono>
#include <cstdlib>
#include <list>
#include <string>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
using std::chrono::system_clock;
using std::chrono::time_point;
using std::list;
using namespace moon::logging;

namespace moon {

ClientSock::ClientSock() {}
ClientSock::ClientSock(int sock) {
  sock_fd = sock;
  last_refresh_time = system_clock::now();
}
bool ClientSock::is_timeout(const seconds &period) {
  if (system_clock::now() < (period + last_refresh_time))
    return false;
  else
    return true;
}

void ClientSock::closesock() {
  log("closing a sock " + sock_fd, INFO);
  close(sock_fd);
}

void ClientSock::refresh_timeout() { last_refresh_time = system_clock::now(); }
int ClientSock::get_sock() { return sock_fd; }

Worker::Worker(LockFreeQueue<int> &external, int maxclients, VirtualMachine &vm)
    : external_clints(external), maxclients(maxclients), vm(vm) {
  timeout_periods = seconds(60 * 60);
}

Worker::~Worker() {
  // free(old);
  delete[] old;
}

list<VmMessage> Worker::serve(list<VdbOp> operations) {
  auto rv = vm.execute(operations);
  return rv;
}

bool Worker::erase_client(int fd) {
  auto ite = internal_client_sockets.begin();
  while (ite != internal_client_sockets.end() && ite->get_sock() != fd)
    ++ite;
  if (ite == internal_client_sockets.end())
    return false;
  pollfds_modified = true;
  internal_client_sockets.erase(ite);
  return true;
}

pollfd *Worker::get_poolfds(size_t *array_len) {
  if (old != nullptr) {
    delete[] old;
  }

  auto size = internal_client_sockets.size();
  pollfd *rv = new pollfd[size];
  auto num = 0;

  for (auto ite = internal_client_sockets.begin();
       ite != internal_client_sockets.end(); ++ite, ++num) {
    rv[num].fd = ite->get_sock();
    rv[num].events = POLLIN;
  }

  old = rv;
  *array_len = num;
  pollfds_modified = false;
  return rv;
}

void Worker::close_timeout_clients() {
  using std::string;
  using std::cout;
  using std::endl;
  cout << "current socks:\t";
  for (auto begin = internal_client_sockets.begin();
       begin != internal_client_sockets.end(); ++begin) {
    auto timeout = begin->is_timeout(timeout_periods);
    cout << begin->get_sock() << "\t";
    if (timeout) {
      begin->closesock();
      internal_client_sockets.erase(begin);
      pollfds_modified = true;
    }
  }
  cout << endl;
}

void Worker::pick_more_clients() {
  while (internal_client_sockets.size() < maxclients) {
    if (internal_client_sockets.size() == 0) { // init pop,might block
      auto new_one = external_clints.pop();
      cout << new_one << endl;
      log("pick up first client,sock " + to_string(new_one), INFO);
      internal_client_sockets.push_back(ClientSock(new_one));
    } else {
      auto new_one = external_clints.try_pop();
      if (new_one.get() == nullptr)
        break;
      cout << new_one << endl;
      log("pick up another client,sock " + to_string(*new_one), INFO);

      internal_client_sockets.push_back(ClientSock(*new_one));
    }
    pollfds_modified = true;
  }
}

void Worker::run() {
  // TODO: test all logic!

  while (true) {
    // pick up more clients
    pick_more_clients();

    // close some timeout sockets
    close_timeout_clients();

    if (internal_client_sockets.size() == 0) // no client to serve
      continue;
    size_t pollsize = 0;
    if (pollfds_modified) { // update fdset, this will free old poolfds
      to_poll = get_poolfds(&pollsize);
    }
    std::cout << "pollsize:" << pollsize << std::endl;
    auto num = poll(to_poll, pollsize, 50);

    if (num != 0) { // serve them

      for (int i = 0; i != pollsize; ++i) {
        bool readyread = to_poll[i].revents & POLLIN;
        bool peerclosed = to_poll[i].revents & POLLRDHUP;
        bool err = to_poll[i].revents & POLLERR;

        if (peerclosed) {

          log("peer has shutdown connection", INFO);
          erase_client(to_poll[i].fd);
          continue;
        } else if (err) {

          log("something err happend,tend to close the connection", ERROR);
          erase_client(to_poll[i].fd);
          continue;

        } else if (readyread) {

          log("prepare to serve a client", INFO);
          static char buffer[1024];

          auto num = recv(to_poll[i].fd, buffer, 1024, 0);
          if (num < 0)
            erase_client(to_poll[i].fd);
          // cout << "recv num " << num << "\t buffer:" << buffer << endl;

          Translater translater;
          auto ops = translater.resp_request_to_vdbop(buffer);
          if (ops.empty()) {
            log(string("nothing to do for client ") + to_string(to_poll[i].fd),
                INFO);
            continue;
          }
          log("serving client " + to_string(to_poll[i].fd), INFO);
          auto messages = serve(ops);
          string response;
          for (auto &message : messages)
            response += translater.message_to_resp_response(message);

          send(to_poll[i].fd, response.c_str(), response.size(), 0);
        }
      }
    }
  }
}
} // namespace moon
