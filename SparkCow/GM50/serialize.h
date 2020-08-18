//
// Created by scott on 7/30/20.
//

#ifndef MONKEY_SERIALIZE_H
#define MONKEY_SERIALIZE_H


#include "datatype.h"

#include "bytearray.h"
#include "attribute.h"

#include <boost/asio.hpp>

inline
ByteStream & operator << (ByteStream& stream,SDM_BYTE v){
    stream.writeByte(v);
    return stream;
}

inline
ByteStream & operator << (ByteStream& stream,SDM_INT16 v){
    stream.writeShort(v);
    return stream;
}

inline
ByteStream & operator << (ByteStream& stream,SDM_UINT16 v){
    stream << SDM_INT16 (v);
    return stream;
}


inline
ByteStream & operator << (ByteStream& stream,SDM_INT32 v){
    stream.writeInt(v);
    return stream;
}

inline
ByteStream & operator << (ByteStream& stream,SDM_UINT32 v){
    stream << SDM_INT32(v);
    return stream;
}

inline
ByteStream & operator << (ByteStream& stream,SDM_UINT64 v){
    stream.writeInt64(int64_t (v));
    return stream;
}

inline
ByteStream & operator << (ByteStream& stream,SDM_INT64 v){
    stream.writeInt64(v);
    return stream ;
}

inline
ByteStream & operator << (ByteStream& stream,SDM_OCTET_STRING v){
    stream.writeInt(v.size());
    stream.writeBytes(v.c_str(),v.size());

    return stream ;
}

inline
void operator >> (ByteStream& stream,AttributeId& v){
    v = stream.readUInt64();
}





#endif //MONKEY_SERIALIZE_H
