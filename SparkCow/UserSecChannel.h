//
// Created by scott on 2020-08-14.
//

#ifndef SMARTBOX_USERSECCHANNEL_H
#define SMARTBOX_USERSECCHANNEL_H

#include "GM50/sechannel.h"

class UserSecChannel:public SecChannel {
public:
    UserSecChannel();

protected:
    void onChannelEstablished();
    void onCheckHealth();
};


#endif //SMARTBOX_USERSECCHANNEL_H
