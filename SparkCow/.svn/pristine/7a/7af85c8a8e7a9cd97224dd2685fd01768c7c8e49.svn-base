//
// Created by scott on 7/29/20.
//

#ifndef MONKEY_MESSAGE_H
#define MONKEY_MESSAGE_H

#include "base.h"
#include "attribute.h"

#define DECL_PARSE(cls) static cls::Ptr parse(const char * data, size_t size);

class Envelope;
class SecChannel;

struct MessageChannelTraverse{
//    typedef std::shared_ptr<Message> Ptr;
    typedef std::shared_ptr< MessageChannelTraverse > Ptr;

    OperationType type;
    Envelope    * envelope;
    SecChannel  * channel;
    MessageChannelTraverse(OperationType type_): type(type_){}

    ~MessageChannelTraverse() { } ;
    virtual void marshall(ByteStream& stream){ };
    virtual std::size_t  getSize(){ return 0; }

//    virtual bool parse(const char* data , std::size_t size){ return false;}
};

struct MessageChannelCreateRequest:MessageChannelTraverse{
    typedef std::shared_ptr< MessageChannelCreateRequest > Ptr;

    std::uint32_t   enc_len;
    ByteArray       enc_data;
    std::uint32_t   sign_len;
    ByteArray       sign_data;

    MessageChannelCreateRequest():MessageChannelTraverse(OperationType::ChannelCreateRequest){

    }
    ~MessageChannelCreateRequest(){}
    void marshall(ByteStream& stream);
    std::size_t  getSize() const ;
//    DECL_PARSE(MessageChannelCreateRequest)
    static MessageChannelCreateRequest::Ptr parse(ByteStream& stream);
};



/// MessageChannelCreateResponse

struct MessageChannelCreateResponse:MessageChannelTraverse{
    typedef std::shared_ptr<MessageChannelCreateResponse > Ptr;

    std::uint32_t   enc_len;
    ByteArray       enc_data;
    std::uint32_t   sign_len;
    ByteArray       sign_data;

    MessageChannelCreateResponse(): MessageChannelTraverse(OperationType::ChannelCreateResponse){

    }

    void marshall(ByteStream& stream);
    static MessageChannelCreateResponse::Ptr parse(ByteStream& stream);
};

/// MessageChannelSend

class Command;
struct MessageChannelSend:MessageChannelTraverse{
    typedef std::shared_ptr< MessageChannelSend > Ptr;

    ByteArray       sign_data;
    std::shared_ptr<Command> detail;
    MessageChannelSend():MessageChannelTraverse(OperationType::ChannelMessageSend){

    }

    void marshall(ByteStream& stream);

    static MessageChannelSend::Ptr parse(ByteStream& stream);
};

/// MessageChannelReCreateRequest

struct MessageChannelReCreateRequest:MessageChannelTraverse{
    typedef std::shared_ptr< MessageChannelReCreateRequest > Ptr;

    MessageChannelReCreateRequest():MessageChannelTraverse(OperationType::ChannelReCreateRequest){

    }

    void marshall(ByteStream& stream);
    static MessageChannelReCreateRequest::Ptr parse(ByteStream& stream);
};


/// MessageTraverseParser

struct MessageTraverseParser{

    static MessageChannelTraverse::Ptr  parse(ByteStream& stream);
};


////


#endif //MONKEY_MESSAGE_H
