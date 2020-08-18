//
// Created by scott2 on 8/11/20.
//

#ifndef MONKEY_USERAPPJSONMESSAGE_H
#define MONKEY_USERAPPJSONMESSAGE_H

#include "../command.h"

// [appid,cmdtype,data_size,data]
// [ 1, 1, 4, N ]

struct CommandUserAppJsonMessage : Command{
    typedef std::shared_ptr<CommandUserAppJsonMessage> Ptr;
    ByteArray  data;
    CommandUserAppJsonMessage():Command(CommandType::UserJsonData, AppID::UserAppExt){

        }

    void marshall( ByteStream& stream);
    static CommandUserAppJsonMessage::Ptr parse(ByteStream& stream);
};


#endif //MONKEY_USERAPPJSONMESSAGE_H
