//
// Created by scott2 on 8/11/20.
//

#include "UserAppJsonMessage.h"


void CommandUserAppJsonMessage::marshall( ByteStream& stream){
    // size
    Command::marshall(stream);
    stream.writeUInt( data.size() );
    stream.writeBytes( data.data(), data.size() );

}

CommandUserAppJsonMessage::Ptr CommandUserAppJsonMessage::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandUserAppJsonMessage>();
    stream.skip(1); //   cmd-type
    std::uint32_t  size;
    size = stream.readUInt();
    cp->data.resize(size);
    stream.readBytes( cp->data.data(),cp->data.size());
    return cp;
}
