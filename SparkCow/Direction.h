//
// Created by scott on 2020-08-14.
//

#ifndef SMARTBOX_DIRECTION_H
#define SMARTBOX_DIRECTION_H

#include "base.h"
#include "GM50/sechannel.h"

class Connector;
class ChannelDirection:public SecChannel{
public:
    typedef std::shared_ptr<ChannelDirection> Ptr;

    virtual ChannelDirection::Ptr against();
    ChannelDirection(bool secure,const std::shared_ptr<Connector>& connector){
        secure_ = secure;
        connector_ = connector;}
    ChannelDirection() {}
    ~ChannelDirection();
    void sendData(const char* data,size_t size);
    void trySendData();
    void onData(const char * data ,size_t size,const Connection::Ptr& conn);
    ByteArray getRemotePubkey(const DeviceID& device_id );
protected:
    std::shared_ptr<Connector> connector_;
    bool    secure_;    //是否加密

    std::list< ByteArray > stream_list;
    std::atomic_bool        connected_;


};
#endif //SMARTBOX_DIRECTION_H
