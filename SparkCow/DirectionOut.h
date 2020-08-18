//
// Created by scott on 2020-08-14.
//

#ifndef SMARTBOX_DIRECTIONOUT_H
#define SMARTBOX_DIRECTIONOUT_H

#include "Direction.h"

class Connector;
class ChannelDirectionOut :public ChannelDirection{
    typedef std::shared_ptr<ChannelDirectionOut> Ptr;
public:
    ChannelDirectionOut(bool secure, const std::shared_ptr<Connector>& connector);
    ChannelDirectionOut(){}
    ~ChannelDirectionOut();

    bool open();
    void close();
    bool init(const Config& props);
    void destroy();
protected:
    void onConnected(const Connection::Ptr& conn);
    void onDisconnected(const Connection::Ptr& conn);
    void onData(const char * data ,size_t size,const Connection::Ptr& conn);
    virtual void onConnectError(const Connection::Ptr& conn,ConnectionError error);  // 0: not reachable , 1: timeout
    void resetStatus();
};


#endif //SMARTBOX_DIRECTIONOUT_H
