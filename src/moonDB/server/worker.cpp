#include "server/worker.h"
#include "translater.h"
#include "vm/vm.h"
#include <chrono>
#include <cstdlib>
#include <list>
#include <sys/poll.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
using std::list;
using std::chrono::time_point;
using std::chrono::system_clock;

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
ClientSock::~ClientSock() { close(sock_fd); }

void ClientSock::refresh_timeout() { last_refresh_time = system_clock::now(); }
int ClientSock::get_sock() { return sock_fd; }

Worker::Worker(LockFreeQueue<int> &external, int maxclients, VirtualMachine &vm)
    : external_clints(external), maxclients(maxclients), vm(vm) {
  timeout_periods = seconds(60 * 60);
}

Worker::~Worker() { free(old); }

list<VmMessage> Worker::serve(list<VdbOp> operations) {
  auto rv = vm.execute(operations);
  return rv;
}

pollfd *Worker::get_poolfds() {
  if (old != nullptr) {
    free(old);
  }
  pollfd *rv =
      (pollfd *)malloc(sizeof(pollfd) * internal_client_sockets.size());
  auto num = 0;
  for (auto ite = internal_client_sockets.begin();
       ite != internal_client_sockets.end(); ++ite, ++num) {
    rv[num].fd = ite->get_sock();
    rv[num].events = POLLIN;
  }
  old = rv;
  return rv;
}

void Worker::run() {
  // TODO: test all logic!
  pollfd *to_poll = nullptr;
  bool modified = false;

  while (true) {
    // pick up more clients
    while (internal_client_sockets.size() < maxclients) {
      auto new_one = external_clints.try_pop();
      if (new_one.get() == nullptr)
        break;
      internal_client_sockets.push_back(ClientSock(*new_one));
      modified = true;
    }

    // close some timeout sockets
    for (auto begin = internal_client_sockets.begin();
         begin != internal_client_sockets.end(); ++begin) {
      if (begin->is_timeout(timeout_periods))
        internal_client_sockets.erase(begin);
      modified = true;
    }

    /* TODO:
        serve them,and refresh timeout
     */
    if (modified) { // update fdset, this will free old poolfds
      to_poll = get_poolfds();
    }
    auto pollsize = sizeof(to_poll) / sizeof(struct pollfd);
    auto num = poll(to_poll, pollsize, 50);
    if (num != 0) { // serve them
      for (int i = 0; i != pollsize; ++i) {
        bool readyread = to_poll[i].revents & POLLIN;
        if (readyread) {
          static char buffer[1024];

          recv(to_poll[i].fd, buffer, 1024, 0);

          Translater translater;
          auto ops = translater.resp_request_to_vdbop(buffer);
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
}
