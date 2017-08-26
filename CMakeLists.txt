project(moonDB)

cmake_minimum_required(VERSION 3.0)

# FOR SOME FLAGS
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -g)

# FOR DIERECTORY
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/moonDB
)

link_directories(
    ${PROJECT_BINARY_DIR}/lib
)



# FOR EXECUTABLE FILES
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
add_subdirectory(src)

# 顺序特别重要!!!!!
