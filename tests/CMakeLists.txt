project(moonDB_TEST)

cmake_minimum_required(VERSION 3.5)

SET(moonDB_DIR ..)
SET(CMAKE_CXX_STANDARD 11)

include_directories(
    ../include/moonDB/
)

link_directories(
    ../build/lib
)


add_executable(test_demo test_translater.cpp )

target_link_libraries(test_demo glog)
target_link_libraries(test_demo libmoonDB.a)

