//
// Created by scott on 8/4/20.
//

#include "RemoteMonitor.h"


void CommandRemoteMonitorRequest::marshall( ByteStream& stream){
    stream.writeByte((std::uint32_t) cmd_type);
    stream.writeBytes(device_id.data(),device_id.size());
    stream.writeInt64(vid);
    stream.writeBytes(params.data(), params.size());

}

CommandRemoteMonitorRequest::Ptr CommandRemoteMonitorRequest::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandRemoteMonitorRequest>();
    stream.skip(1);
    stream.readBytes( cp->device_id.data(),cp->device_id.size());
    cp->vid = stream.readUInt64();
    stream.readBytes( (char*)&cp->params[0],14);
    return cp;
}

//
//void CommandRemoteMonitorResponse::marshall( ByteStream& stream){
//
//}

CommandRemoteMonitorResponse::Ptr CommandRemoteMonitorResponse::parse(ByteStream& stream){
    CommandRemoteMonitorRequest::Ptr cp = CommandRemoteMonitorRequest::parse(stream);
    return std::dynamic_pointer_cast<CommandRemoteMonitorResponse>(cp);
}
