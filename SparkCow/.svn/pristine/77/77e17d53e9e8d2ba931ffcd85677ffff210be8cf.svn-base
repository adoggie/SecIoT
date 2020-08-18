//
// Created by scott on 7/31/20.
//

#include "command.h"
#include "serialize.h"
#include <map>

#include "apps/ComplianceTest.h"
#include "apps/ParamConfigure.h"
#include "apps/RemoteMaintenance.h"
#include "apps/RemoteMonitor.h"
#include "apps/SecKeyManagement.h"
#include "apps/VpnManagement.h"
#include "apps/UserAppJsonMessage.h"

//std::vector<ParseFuncCommand> parse_func_list_command={
//        CommandGet::parse,
//        CommandGetNext::parse,
//        CommandGetBulk::parse,
//        CommandSet::parse,
//        CommandInform::parse,
//        CommandTrap::parse
//};

std::map< CommandType, ParseFuncCommand> CommandParseFuncList = {
        { CommandType::Get , CommandGet::parse},
        { CommandType::GetNext , CommandGetNext::parse},
        { CommandType::GetBulk, CommandGetBulk::parse},
        { CommandType::Response, CommandResponse::parse},
        { CommandType::Set, CommandSet::parse},
        { CommandType::Inform, CommandInform::parse},
        { CommandType::Trap, CommandTrap::parse},

        { CommandType::RemoteMonitorRequest, CommandRemoteMonitorRequest::parse},
        { CommandType::RemoteMonitorResponse, CommandRemoteMonitorResponse::parse},

        { CommandType::ComplianceTestRequest, CommandRemoteMonitorRequest::parse},
        { CommandType::ComplianceTestResponse, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementNegotiateRequest, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementNegotiateResponse, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementDistributeRequest, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementDistributeResponse, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementDestroyRequest, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementDestroyResponse, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementEnableRequest, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementEnableResponse, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagementApplyRequest, CommandRemoteMonitorRequest::parse},
        { CommandType::SecKeyManagemenApplyResponse, CommandRemoteMonitorRequest::parse},

        { CommandType::UserJsonData, CommandUserAppJsonMessage::parse},

};

/// Command

void Command::marshall( ByteStream& stream){
    stream.writeByte(std::uint8_t ( app_id) );
    stream.writeByte( std::uint8_t(cmd_type));
}

Attribute::Ptr Command::readAttribute(ByteStream& stream){
    Attribute::Ptr  attr = Attribute::parse(stream);
    return attr;
}

std::vector< Attribute::Ptr > Command::readMultiAttributes(ByteStream& stream , size_t n){
    size_t size;
    AttributeId  aid;

    std::vector< Attribute::Ptr > attrs;

    std::uint32_t  attr_val_size = 4 ;
    std::uint32_t  attr_id_size = sizeof(AttributeId);
    while(true){
        if( stream.remain() <= attr_id_size +  attr_val_size){
            break ;
        }
        auto attr = this->readAttribute(stream);
        attrs.push_back(attr);
    }
    return attrs;
}

/// CommandGet

void CommandGet::marshall( ByteStream& stream){
    Command::marshall(stream);
    for( auto & aid : aids){
        stream << aid;
    }
}


CommandGet::Ptr CommandGet::parse(ByteStream& stream){

    CommandGet::Ptr cp = std::make_shared<CommandGet>();
//    ByteStream stream(data,size);

    AttributeId aid;

    cp->readHead(stream);
// todo
//    for(size_t n=0 ; n<  size/ (sizeof (AttributeId)) ; n++){
//        stream >> aid;
//        cp->aids.push_back( aid );
//    }
    return cp;
}

/// CommandGetNext

CommandGetNext::Ptr CommandGetNext::parse(ByteStream& stream){
    return std::dynamic_pointer_cast<CommandGetNext>(CommandGet::parse(stream));
}

/// CommandGetBulk

void CommandGetBulk::marshall( ByteStream& stream){
    Command::marshall(stream);
    stream.writeByte(this->n);
    stream.writeByte(this->m);
    for( auto & aid : aids){
        stream << aid;
    }
}

CommandGetBulk::Ptr CommandGetBulk::parse(ByteStream& stream){
    CommandGetBulk::Ptr cp = std::make_shared<CommandGetBulk>();
//    ByteStream stream(data,size);
    cp->n = stream.readByte();
    cp->m = stream.readByte();

    AttributeId aid;
// todo
//    size +=2;
//    for(size_t n=0 ; n<  size / (sizeof (AttributeId)) ; n++){
//        stream >> aid;
//        cp->aids.push_back( aid );
//    }
    return cp;

}

/// CommandResponse

void CommandResponse::marshall( ByteStream& stream){
    Command::marshall(stream);
    stream.writeByte(this->status);

    for( auto & attr : attrs){
        if( this->status == Okay){
            stream << attr->aid;
            auto type = AttributeTable::get( attr->aid)->type;
            std::uint32_t len = std::uint32_t (type);
            if( type == AttributeValueType::String){
                auto ss = boost::any_cast<std::string>(attr->value);
                len = ss.size();
            }else{

            }
        }
    }
}

CommandResponse::Ptr CommandResponse::parse(ByteStream& stream){
    CommandResponse::Ptr cp = std::make_shared<CommandResponse>();
//    ByteStream stream(data,size);
    cp->readHead(stream);
    cp->status = stream.readByte();

    cp->readMultiAttributes(stream);
    AttributeId aid;

//    for(size_t n=0 ; n<  size / (sizeof (AttributeId)) ; n++){
//        stream >> aid;
//        cp->aids.push_back( aid );
//    }
    return cp;
}

/// CommandSet
/// \param stream
void CommandSet::marshall( ByteStream& stream) {
    Command::marshall(stream);
    for(auto & attr: attrs){
        attr->marshall(stream);
    }
}

CommandSet::Ptr CommandSet::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandSet>();
    cp->readHead(stream);
    while (stream.remain()){
        Attribute::Ptr attr;
        attr = Attribute::parse(stream);
        cp->attrs.push_back(attr);
    }
    return cp ;
}

/// CommandInform
void CommandInform::marshall( ByteStream& stream) {
    Command::marshall(stream);
    stream.writeByte(type);
    for(auto & attr: attrs){
        attr->marshall(stream);
    }
}

CommandInform::Ptr CommandInform::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandInform>();
    cp->readHead(stream);
    cp->type = stream.readByte();
    while (stream.remain()){
        Attribute::Ptr attr;
        attr = Attribute::parse(stream);
        cp->attrs.push_back(attr);
    }
    return cp ;
}

/// CommandTrap
/// \param stream

void CommandTrap::marshall( ByteStream& stream){
    CommandInform::marshall(stream);
}

CommandTrap::Ptr CommandTrap::parse(ByteStream& stream){
    auto cp = std::make_shared<CommandTrap>();
    cp->readHead(stream);
    cp->type = stream.readByte();
    while (stream.remain()){
        Attribute::Ptr attr;
        attr = Attribute::parse(stream);
        cp->attrs.push_back(attr);
    }
    return cp ;
}


/// CommandListener








