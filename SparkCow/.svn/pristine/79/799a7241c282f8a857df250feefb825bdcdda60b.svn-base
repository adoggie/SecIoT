//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_SECKEYMANAGEMENT_H
#define MONKEY_SECKEYMANAGEMENT_H

#include "../command.h"
#include "../base.h"

struct AtomKeyID{
    std::uint16_t  manager_id;
    std::uint16_t  agent_id;
    std::uint32_t  key_no;
    AtomKeyID(){
        manager_id = agent_id = key_no =0;
    }
    void marshall(ByteStream& stream){
        stream.writeShort((std::int16_t )manager_id);
        stream.writeShort( (std::int16_t) agent_id);
        stream.writeUInt( key_no);
    }

    void parse(ByteStream& stream){
        manager_id =(std::uint16_t) stream.readShort();
        agent_id = (std::uint16_t) stream.readShort();
        key_no = stream.readUInt();
    }

    size_t getSize(){
        return 8;
    }
};

/// GM51/7.2.2
struct SecKeyBody{
    AtomKeyID       key_id;
    std::uint32_t  key_type;
    std::uint32_t   key_len;
    ByteArray       key_set;
    std::uint32_t   key_check_val_alg;
    std::uint8_t    key_check_val_len;
    ByteArray       key_check_val;
    std::uint32_t   key_sys_id;
    std::uint32_t   key_device_id;

    size_t getSize(){
        size_t  size = 0;
        size += key_id.getSize() + 21 + key_set.size() + key_check_val.size();
        return size;
    }
    void marshall(ByteStream& stream){
        key_id.marshall(stream);
        stream.writeUInt(key_type);
        stream.writeUInt(key_len);
        stream.writeBytes(key_set.data(),key_len);
        stream.writeUInt(key_check_val_alg);
        stream.writeByte(key_check_val_len);
        stream.writeBytes(key_check_val.data(),key_check_val_len);
        stream.writeUInt(key_sys_id);
        stream.writeUInt(key_device_id);
    }

    void parse(ByteStream& stream){
        key_id.parse(stream);
        key_type = stream.readUInt();
        key_len = stream.readUInt();
        key_set.resize(key_len);
        stream.readBytes(key_set.data(),key_set.size());
        key_check_val_alg = stream.readUInt();
        key_check_val_len = stream.readByte();
        key_check_val.resize(key_check_val_len);
        stream.readBytes( key_check_val.data() , key_check_val.size());
        key_sys_id = stream.readUInt();
        key_device_id = stream.readUInt();
    }
};

/// GM51/7.2.1


struct CommandSecKeyBase:Command{
    typedef std::shared_ptr<CommandSecKeyBase> Ptr;

    std::uint8_t   ver;
    CommandSecKeyBase(CommandType type):Command(type,AppID::SecKeyManagement){

    }

    virtual void marshall( ByteStream& stream){
//        Command::marshall(stream);
        stream.writeByte(std::uint8_t ( app_id) );
        stream.writeByte(ver);
        stream.writeByte( std::uint8_t(cmd_type));
    }

    void readHead( ByteStream& stream){
        stream.skip(1);
        ver = stream.readByte();
        stream.skip(1);
    }
};

struct CommandSecKeyNegotiateRequest:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyNegotiateRequest> Ptr;

    std::uint32_t  len;
    std::uint8_t   algorithm_id;
    std::uint32_t  key_len;
    ByteArray       key_data;

public :
    CommandSecKeyNegotiateRequest():CommandSecKeyBase(CommandType::SecKeyManagementNegotiateRequest){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyNegotiateRequest::Ptr parse(ByteStream& stream);
};

struct CommandSecKeyNegotiateResponse:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyNegotiateResponse> Ptr;
    std::uint32_t  len;
    std::uint32_t  data;
public :
    CommandSecKeyNegotiateResponse():CommandSecKeyBase(CommandType::SecKeyManagementNegotiateResponse){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyNegotiateResponse::Ptr parse(ByteStream& stream);
};

struct CommandSecKeyDistributeRequest:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyDistributeRequest> Ptr;
    std::uint32_t  len;
    std::uint32_t   key_num;
//    ByteArray  data;
    std::vector<SecKeyBody>  dist_Keys;
public :
    CommandSecKeyDistributeRequest():CommandSecKeyBase(CommandType::SecKeyManagementDistributeRequest){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyDistributeRequest::Ptr parse(ByteStream& stream);
};

struct CommandSecKeyDistributeResponse:    CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyDistributeResponse> Ptr;
    std::uint32_t len;
    std::uint32_t  result; // 0: no error ,  >0 and < KMR_BASE: num succ ,  > KMR_BASE : all failed
    std::vector<AtomKeyID> dist_keys;
public :
    CommandSecKeyDistributeResponse():CommandSecKeyBase(CommandType::SecKeyManagementDistributeResponse){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyDistributeResponse::Ptr parse(ByteStream& stream);
};

// GM51/7.2.6
/// GM51/7.2.6  destroy all keys
/// No Parameters
struct CommandSecKeyDestroyRequest:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyDestroyRequest> Ptr;
public :
    CommandSecKeyDestroyRequest():CommandSecKeyBase(CommandType::SecKeyManagementDestroyRequest){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyDestroyRequest::Ptr parse(ByteStream& stream);
};


/// GM51/7.2.6  destroy all keys
struct CommandSecKeyDestroyResponse:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyDestroyResponse> Ptr;
    std::uint32_t  len;
    std::uint32_t  result;
public :
    CommandSecKeyDestroyResponse():CommandSecKeyBase(CommandType::SecKeyManagementDestroyResponse){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyDestroyResponse::Ptr parse(ByteStream& stream);
};

// GM51 /7.2.7
struct CommandSecKeyEnableRequest:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyEnableRequest> Ptr;
    std::uint32_t  len; // 0: enalbe all
    std::uint8_t  flag;
    std::vector< std::uint32_t > key_ids;
public :
    CommandSecKeyEnableRequest():CommandSecKeyBase(CommandType::SecKeyManagementEnableRequest){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyEnableRequest::Ptr parse(ByteStream& stream);
};

struct CommandSecKeyEnableResponse:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyEnableResponse> Ptr;

    std::uint32_t  len; // 0: enalbe all
    std::uint32_t  result;
    std::vector< std::uint32_t > key_ids;
public :
    CommandSecKeyEnableResponse():CommandSecKeyBase(CommandType::SecKeyManagementEnableResponse){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyEnableResponse::Ptr parse(ByteStream& stream);
};

/// GM51/7.2.8
struct CommandSecKeyApplyRequest:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyApplyRequest> Ptr;
    std::uint32_t  len;
    std::uint32_t  key_type;
    std::uint32_t  key_num;
public :
    CommandSecKeyApplyRequest():CommandSecKeyBase(CommandType::SecKeyManagementApplyRequest){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyApplyRequest::Ptr parse(ByteStream& stream);
};

struct CommandSecKeyApplyResponse:CommandSecKeyBase{
    typedef std::shared_ptr<CommandSecKeyApplyResponse> Ptr;
    std::uint32_t  len;
    std::uint32_t  result;
    std::uint32_t  key_num;
    std::vector<SecKeyBody> key_list;
public :
    CommandSecKeyApplyResponse():CommandSecKeyBase(CommandType::SecKeyManagemenApplyResponse){

    }

    void marshall( ByteStream& stream);
    static CommandSecKeyApplyResponse::Ptr parse(ByteStream& stream);
};


class SecKeyManagement {

public:


};


#endif //MONKEY_SECKEYMANAGEMENT_H
