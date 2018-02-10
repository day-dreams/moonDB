#ifndef _OPCODE
#define _OPCODE

#include "types.h"

#include <cstring>

namespace moon
{

namespace OPCODE
{
const u8 SET = 0;     /* [set key value]，添加/覆盖一个key-value */
const u8 GET = 1;     /* [get key]，获取key的value */
const u8 DEL = 2;     /* [del key]，删除key */
const u8 EXISTED = 3; /* [existed key]，查询key是否存在 */

/*
    str_to_opcode will read c string which represent a type of an operation,then
   decode it to u8.
    c string is in [begin,end),and num of chars which have been read will be
   returned by size_read.
 */
u8 str_to_opcode(const char *begin, const char *end, i32 *size_read);
}
}

#endif