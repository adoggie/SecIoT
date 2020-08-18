
#ifndef GM50_UTILS_H
#define GM50_UTILS_H

#include <stdlib.h>
#include <cstdint>

#include <boost/asio.hpp>

#include "bytearray.h"



inline
uint64_t ntohl64(uint64_t arg64){

    uint64_t res64;

#if __LITTLE_ENDIAN
    uint32_t low = (uint32_t) (arg64 & 0x00000000FFFFFFFFLL);
    uint32_t high = (uint32_t) ((arg64 & 0xFFFFFFFF00000000LL) >> 32);

    low = ntohl(low);
    high = ntohl(high);

    res64 = (uint64_t) high + (((uint64_t) low) << 32);
#else
    res64 = arg64;
#endif

    return res64;

}

inline
uint64_t htonl64(uint64_t arg64){

    uint64_t res64;

#if __LITTLE_ENDIAN
    uint32_t low = (uint32_t) (arg64 & 0x00000000FFFFFFFFLL);
    uint32_t high = (uint32_t) ((arg64 & 0xFFFFFFFF00000000LL) >> 32);

    low = htonl(low);
    high = htonl(high);

    res64 = (uint64_t) high + (((uint64_t) low) << 32);
#else
    res64 = arg64;
#endif

    return res64;
}



//ByteStream& operator << (ByteStream&,SDM_DateAndTime& dt);


#endif