
#ifndef INNERPROC_UTILS_H
#define INNERPROC_UTILS_H

#include <string>
#include <string>
#include <sstream>
#include <vector>

#include "base.h"

namespace utils {

#define YELLOW "\u001b[93m"
#define GREEN "\u001b[32m"
#define WHITE "\u001b[37m"
#define RED "\u001b[31m"


    std::string generateUUID();
    std::string getIpAddress(const std::string& eth="eth0");

    typedef std::vector<char > ByteArray;
    int fromHex( char c ) ;
    std::uint8_t fromHex( const char *c ) ;
    ByteArray convertHex2Bin(const char* data,size_t size);
    std::string toHex(const void* inRaw, int len);
    std::string toHexLower(const void* inRaw, int len) ;


    std::string generateSignature(const std::string& secret_key,const std::string& text);

    std::string getDeviceUniqueId();
    bool writeDeviceUniqueId(const std::string& uid);

    //设置网卡地址
    bool setNicIpAddress(const std::string& nic,const std::string& ip );

    // 收集
    void collectDeviceStatus(const std::string & script,PropertyStringMap & kvs);

    void toByteArray(const char * data, size_t size , ByteArray& bytes);
    void toByteArray(const std::string& data , ByteArray& bytes);

    template <typename T>
    inline void vector_append(T& a, const T& b ) {
        T _temp = T(a.size()+ b.size());
        std::copy_n(a.data(),a.size(),_temp.begin());
        std::copy_n(b.data(),b.size(),_temp.begin()+ a.size());
        a = _temp;
    }

}

#endif
