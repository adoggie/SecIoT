//
// Created by scott on 7/30/20.
//

#ifndef MONKEY_ByteStream_H
#define MONKEY_ByteStream_H

#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/asio.hpp>
#include "utils.h"
#include "error.h"
#include "../utils.h"

class ByteStream{
protected:
    std::vector<uint8_t> bytes_;
    size_t pos_; //当前位置
public:

    ByteStream(){
        reset();
    }

    ByteStream(const char* data,size_t size){
        reset();
        bytes_.resize(size);
        bytes_.assign((uint8_t*)data,(uint8_t*)(data+size) );

    }

    std::vector<uint8_t>& detail(){
        return bytes_;
    }
    size_t size() const{
        return bytes_.size();
    }

    char * data(){
        return (char*)&bytes_[0];
    }

    char * current(){
        return (char*)&bytes_[pos_];
    }

    void reset(){
        bytes_.clear();
        pos_ = 0;
    }

    void erase( size_t offset, size_t size){
        bytes_.erase( bytes_.begin()+offset,bytes_.begin()+offset + size);
    }

    void position(int pos){
        pos_ = pos;
    }

    int remain(){
        return size() - pos_;
    }

    void next(int p){
        pos_+=p;
    }

    void skip(int np){
        next(np);
    }

    int position() const{
        return pos_;
    }



    void writeByte(uint8_t val) {
        bytes_.push_back(val);
    }

    void writeShort(int16_t val){
        val = htons(val);

//        bytes_.resize(bytes_.size() + 2);
//        memcpy(&bytes_[bytes_.size()-2],&val,2);

        std::copy_n((char*)&val,2,std::back_inserter(bytes_));
    }

    void writeInt(int32_t val) {
        val = boost::asio::detail::socket_ops::host_to_network_long(val);

//        bytes_.resize(bytes_.size() + 4);
//        memcpy(&bytes_[bytes_.size()-4],&val,4);

        std::copy_n((char*)&val,4,std::back_inserter(bytes_));
    }

    void writeUInt(uint32_t val) {
        writeInt(int32_t(val));
    }

    void writeUnsignedInt(uint32_t val) {
        writeInt((int32_t)val);
    }

    void writeInt64(int64_t val) {
        val= htonl64(val);

//        bytes_.resize(bytes_.size() + 8);
//        memcpy(&bytes_[bytes_.size()-8],&val,8);

        std::copy_n((char*)&val,8,std::back_inserter(bytes_));
    }

    void writeUInt64(uint64_t val) {
        writeInt64(int64_t(val));
    }

    void writeFloat(float val){
        int v ;
        memcpy(&v,&val,4);
        writeInt(v);
    }

    void writeDouble(double val){
        int64_t v;
        memcpy(&v,&val,8);

//        bytes_.resize(bytes_.size() + 8);
//        memcpy(&bytes_[bytes_.size()-8],&v,8);

        std::copy_n((char*)&val,8,std::back_inserter(bytes_));
    }

    void writeString(const std::string& d) {
        size_t size = d.size();
        this->writeInt((int32_t)size);

//        bytes_.resize(bytes_.size()+size);
//        memcpy(&bytes_[bytes_.size() - size],(uint8_t*)d.c_str(),size);

        std::copy_n((char*)d.c_str(),size,std::back_inserter(bytes_));

    }

    void writeBytes(const ByteStream& d) {
        size_t size = d.size();

//        bytes_.resize(bytes_.size()+size);
//        memcpy(&bytes_[bytes_.size() - size],&d.bytes_[0],size);

        std::copy_n((char*)&d.bytes_[0],size,std::back_inserter(bytes_));
    }

    void writeBytes(const ByteStream& d,int pos,size_t size ){
//        bytes_.resize(bytes_.size()+size);
//        memcpy(&bytes_[bytes_.size() - size],&d.bytes_[pos],size);

        std::copy_n((char*)&d.bytes_[pos],size,std::back_inserter(bytes_));
    }

    void writeBytes(const char* d,size_t size ) {
//        bytes_.resize(bytes_.size()+size);
//        memcpy(&bytes_[bytes_.size() - size],d,size);

        std::copy_n((char*)d,size,std::back_inserter(bytes_));
    }

/*
	void writeBytes(const ByteStream& d,size_t pos,size_t len) throw(Exception){
		size_t size = bytes_.size();
		bytes_.resize(bytes_.size()+size);
		memcpy(&bytes_[bytes_.size() - size],&d.bytes_[0],d.bytes_.size());
	}
*/
    uint8_t readByte() {
        uint8_t r;
        if(pos_+1  > bytes_.size() ){
            throw Exception(Consts::DATA_INSUFFICIENT);
        }
        r = bytes_[pos_];
        pos_+=1;
        return r;
    }

    int16_t readShort() throw (Exception){
        uint16_t r;
        if(pos_+ 2  > bytes_.size() ){
            throw Exception(Consts::DATA_INSUFFICIENT);
        }
        memcpy(&r,&bytes_[pos_],2);
        r = ntohs(r);
        pos_+=2;
        return r;
    }

    int32_t readInt() throw (Exception){
        int32_t r;
        if(pos_+ 4  > bytes_.size() ){
            throw Exception(Consts::DATA_INSUFFICIENT);
        }
        memcpy(&r,&bytes_[pos_],4);
        r = ntohl(r);
        pos_+=4;
        return r;
    }

    uint32_t readUnsignedInt() throw (Exception){
        uint32_t r;
        r = (uint32_t) readInt();
        return r;
    }

    uint32_t readUInt(){
        return readUnsignedInt();
    }

    int64_t readInt64() throw(Exception){
        int64_t r;
        if(pos_+ 8  >bytes_.size() ){
            throw Exception(Consts::DATA_INSUFFICIENT);
        }
        memcpy(&r,&bytes_[pos_],8);
        r =(int64_t) ntohl64((uint64_t)r);
        pos_+=8;
        return r;
    }

    uint64_t readUInt64() throw(Exception){
        return (uint64_t )readInt64();
    }


    float readFloat() throw(Exception){
        float r;
        int32_t ir;
        ir = readInt();
        memcpy(&r,&ir,4);
        return r;
    }

    double readDouble() throw(Exception){
        double r;
        int64_t i64;
        i64 = readInt64();
        memcpy(&r,&i64,8);
        return r;
    }

    std::string readString() throw(Exception){
        std::string r;
        int32_t size;
        size = readInt();

        if(pos_+ size  > bytes_.size() ){
            throw Exception(Consts::DATA_INSUFFICIENT);
        }
        r.assign((char*)&bytes_[pos_],size);
        pos_+=size;

        return r;
    }

    void readBytes(char * data ,size_t size){
        memcpy(data,(char*)&bytes_[pos_] , size);
        pos_+= size;
    }


};
#endif //MONKEY_ByteStream_H
