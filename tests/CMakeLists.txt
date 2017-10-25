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

# SET(TEST_SRC test_translater)
# SET(TEST_SRC test_vm.cpp)
# SET(TEST_SRC test_inetaddr.cpp)
# SET(TEST_SRC test_tcpserver)
# SET(TEST_SRC test_tcpclient)


SET(TEST_SRC server)
add_executable(${TEST_SRC}.out ${moonDB_DIR}/src/moonDB/server/${TEST_SRC}.cpp )
target_link_libraries(${TEST_SRC}.out pthread)
target_link_libraries(${TEST_SRC}.out libmoonDB.a)
add_executable(demo.out ${moonDB_DIR}/src/moonDB/server/${TEST_SRC}.cpp )
target_link_libraries(demo.out pthread)
target_link_libraries(demo.out libmoonDB.a)




# for tests
# add_executable(${TEST_SRC}.out ${TEST_SRC}.cpp )
# target_link_libraries(${TEST_SRC}.out pthread)
# target_link_libraries(${TEST_SRC}.out libmoonDB.a)

# add_executable(demo.out ${TEST_SRC}.cpp )
# target_link_libraries(demo.out pthread)
# target_link_libraries(demo.out libmoonDB.a)
