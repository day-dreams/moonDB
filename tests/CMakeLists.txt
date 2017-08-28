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
SET(TEST_SRC test_vm.cpp)

add_executable(demo.out ${TEST_SRC} )

target_link_libraries(demo.out glog)
target_link_libraries(demo.out libmoonDB.a)

