//
// Created by scott on 2020-08-14.
//

#ifndef SMARTBOX_DIRECTIONIN_H
#define SMARTBOX_DIRECTIONIN_H

#include "Direction.h"

class Connector;
class ChannelDirectionIn:public ChannelDirection {
public:
    typedef std::shared_ptr<ChannelDirectionIn> Ptr;

    ChannelDirectionIn(bool secure,
            const std::shared_ptr<Connector>& connector);
    ChannelDirectionIn(){}
    ~ChannelDirectionIn();
    bool open(const Connection::Ptr& conn );
    void close();
    bool init(const Config& props);
protected:
    void onConnected(const Connection::Ptr& conn);
    void onDisconnected(const Connection::Ptr& conn);
    void onData(const char * data ,size_t size,const Connection::Ptr& conn);
    virtual void onConnectError(const Connection::Ptr& conn,ConnectionError error);  // 0: not reachable , 1: timeout
    void resetStatus();

protected:
    virtual void onMessageChannelCreateRequest(const MessageChannelCreateRequest::Ptr& message);
    virtual void onMessageChannelCreateResponse(const MessageChannelCreateResponse::Ptr& message);
    virtual void onMessageChannelSend(const MessageChannelSend::Ptr& message);
    virtual void onMessageChannelReCreateRequest(const MessageChannelReCreateRequest::Ptr& message);

};


#endif //SMARTBOX_DIRECTIONIN_H
