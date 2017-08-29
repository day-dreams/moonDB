PROJECT(moonDB)

SET(moonDB_SRCS
    vm/vm.cpp
    db/engine.cpp
    translater.cpp
    VdbOp.cpp
    opcode.cpp
    InetAddr.cpp
)


# shared library
add_library(moonDB SHARED ${moonDB_SRCS})


# static library
add_library(moonDB_static ${moonDB_SRCS})
SET_TARGET_PROPERTIES(moonDB_static PROPERTIES OUTPUT_NAME "moonDB")
SET_TARGET_PROPERTIES(moonDB PROPERTIES CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES(moonDB_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)