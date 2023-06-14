#ifndef DBCHeader_HPP
#define DBCHeader_HPP
#include <string>
#include <fstream>
#include <cstring>
#include <map>
using stringref = uint32_t;
struct DBCHeader
{
    uint32_t head; // always 'WDBC'
    uint32_t recordCount; // records per file
    uint32_t fieldCount; // fields per record
    uint32_t recordSize; // sum (sizeof (field_type_i)) | 0 <= i < field_count. field_type_i is NOT defined in the files.
    uint32_t stringBlockSize;
};
#endif