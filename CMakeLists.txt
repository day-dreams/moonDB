cmake_minimum_required(version 3.0)
project(moonDB)

# FOR SOME FLAGS
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS -g)

# FOR DIERECTORY
add_subdirectory(src)
add_subdirectory(include)

# FOR EXECUTABLE FILES

# FOR 3RD LIBRARY
target_link_libraries()

