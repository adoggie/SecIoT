//
// Created by scott on 2020-08-17.
//

#ifndef SMARTBOX_INPROCDIRECTIONIN_H
#define SMARTBOX_INPROCDIRECTIONIN_H

#include "../DirectionIn.h"
#include "ContextConnection.h"

class InProcDirectionIn:public ChannelDirectionIn {
public:
    InProcDirectionIn(const ContextConnection::Ptr & conn){
        conn_ = conn;
    }

    bool init(const Config& props);
    void initCommandListener();
    ContextConnection::Ptr getConnection(){ return std::dynamic_pointer_cast<ContextConnection>(conn_);}
};


#endif //SMARTBOX_INPROCDIRECTIONIN_H
