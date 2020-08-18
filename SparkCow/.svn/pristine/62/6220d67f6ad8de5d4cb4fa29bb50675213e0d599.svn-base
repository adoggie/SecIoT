//
// Created by scott on 7/29/20.
//

#ifndef MONKEY_ENVELOPE_H
#define MONKEY_ENVELOPE_H

#include "message.h"
#include "bytearray.h"


class SecChannel;

struct Envelope{
    typedef std::shared_ptr< Envelope> Ptr;



    std::uint8_t    version;       ///
    std::uint8_t    safe_mode;
    std::uint16_t   not_used;
    std::uint64_t      msg_id;
    std::uint32_t   pdu_len;
    DeviceID        target;
    DeviceID        source;
    OperationType    op_type;
    std::uint8_t    app_id;
    MessageChannelTraverse::Ptr    detail;
    std::uint32_t   sign_len;
    ByteArray       sign_data;

    SecChannel  *  channel;

    void marshall(ByteStream& stream);
    void finish(ByteStream& stream,ByteArray sign_data);
//    MessageTraverse::Ptr  getMessage();

    // error: 0 succ , others false
    // return: message array

private:

};

typedef std::vector<Envelope::Ptr> EnvelopeArray;

class SecChannel;

class  EnvelopeManager{
    DECL_SINGLETON(EnvelopeManager)
public:
    Envelope::Ptr createEnvelope();
    std::uint64_t  generateNextSequence();
    int enqueue(const char * data, size_t size,EnvelopeArray& evps );
    void reset();
    SecChannel* getSecChannel(){ return channel_; };
    void setSecChannel(SecChannel*  channel){ channel_ = channel;}
protected:
    int split(const char * data ,size_t size , size_t & consumed,std::vector< std::shared_ptr<ByteStream> > & bulks);


    Envelope::Ptr parse(const char *  data ,size_t size);

private:
//    std::vector<char> B;
    ByteStream buff_;
    std::uint64_t   sequence_;
    SecChannel* channel_;
};


#endif //MONKEY_ENVELOPE_H
