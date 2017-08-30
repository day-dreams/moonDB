project(moonDB_TEST)

cmake_minimum_required(VERSION 3.5)

SET(moonDB_DIR ..)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_FLAGS -g)

include_directories(
    ../include/moonDB/
)

link_directories(
    ../build/lib
)

# SET(TEST_SRC test_translater.cpp)
# SET(TEST_SRC test_vm.cpp)
# SET(TEST_SRC test_inetaddr.cpp)
# SET(TEST_SRC test_tcpserver)
SET(TEST_SRC test_tcpclient)

add_executable(${TEST_SRC}.out ${TEST_SRC}.cpp )

target_link_libraries(${TEST_SRC}.out glog)
target_link_libraries(${TEST_SRC}.out libmoonDB.a)

