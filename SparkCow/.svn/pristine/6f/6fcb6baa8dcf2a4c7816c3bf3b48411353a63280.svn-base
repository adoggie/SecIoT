//
// Created by scott2 on 8/11/20.
//

#ifndef MONKEY_USERAPPJSONDATAIMPL_H
#define MONKEY_USERAPPJSONDATAIMPL_H

#include <vector>
#include "../GM50/apps/UserAppJsonMessage.h"
#include "../message.h"

class UserAppJsonDataImpl :public CommandListener{
public:
    typedef std::shared_ptr<UserAppJsonDataImpl> Ptr;
    void onCommand(const Command::Ptr& command);
    void reset();
protected:
    void onDeviceStatusQuery(const std::shared_ptr<MessageDeviceStatusQuery>& query);
    void onDeviceValueSet(const std::shared_ptr<MessageDeviceValueSet>& msg);
    void parseMessage(const char* data ,size_t size);
private:
    ByteArray buff_;
};


#endif //MONKEY_USERAPPJSONDATAIMPL_H
