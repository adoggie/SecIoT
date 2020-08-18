//
// Created by scott on 7/31/20.
//

#ifndef MONKEY_COMMAND_H
#define MONKEY_COMMAND_H

#include "base.h"
#include "attribute.h"
#include "bytearray.h"


class SecChannel;
struct Command{
    typedef std::shared_ptr< Command > Ptr;
    SecChannel *  channel;

    Command(){}
    Command(CommandType type_,AppID app_id_=AppID::DeviceManagement){
        app_id = app_id_;
        cmd_type = type_;
    }
    virtual void marshall( ByteStream& stream);

    AppID  app_id;
    CommandType  cmd_type;

//    void parse()
    void readHead(ByteStream& stream){
        app_id = (AppID)stream.readByte();
        cmd_type = (CommandType)stream.readByte();
    }

    Attribute::Ptr readAttribute(ByteStream& stream);
    std::vector< Attribute::Ptr > readMultiAttributes(ByteStream& stream , size_t n=0);
};

struct CommandGet:Command{
    typedef std::shared_ptr< CommandGet > Ptr;

    std::vector<AttributeId>  aids;
    CommandGet():Command(CommandType::Get){}
    void marshall( ByteStream& stream);
    static CommandGet::Ptr parse(ByteStream& stream);
};

struct CommandGetNext:CommandGet{
    typedef std::shared_ptr< CommandGetNext > Ptr;

    CommandGetNext(){
        cmd_type = CommandType::GetNext;
    }
//    void marshall( ByteStream& stream);
    static CommandGetNext::Ptr parse(ByteStream& stream);
};

struct CommandGetBulk:Command{
    typedef std::shared_ptr< CommandGetBulk > Ptr;

    std::uint8_t  n;
    std::uint8_t  m;
    std::vector<AttributeId>  aids;
    CommandGetBulk(): Command(CommandType::GetBulk){

    }

    void marshall( ByteStream& stream);
    static CommandGetBulk::Ptr parse(ByteStream& stream);
};

struct CommandResponse:Command{
    typedef std::shared_ptr<CommandResponse> Ptr;

    enum {
        Okay = 0 ,
        Error = 1
    };


    std::uint8_t   status;
    std::vector< Attribute::Ptr>  attrs;

    CommandResponse():Command(CommandType::Response){
        status = Okay;
    }

    void marshall( ByteStream& stream);
    static CommandResponse::Ptr parse(ByteStream& stream);
};


struct CommandSet:Command{
    typedef std::shared_ptr<CommandSet> Ptr;

    std::vector< Attribute::Ptr>  attrs;

    CommandSet():Command(CommandType::Set){

    }
    void marshall( ByteStream& stream);
    static CommandSet::Ptr parse(ByteStream& stream);
};


struct CommandInform:Command{
    typedef std::shared_ptr<CommandInform> Ptr;

    std::uint8_t  type;
    std::vector< Attribute::Ptr>  attrs;

    CommandInform():Command(CommandType::Inform){

    }
    void marshall( ByteStream& stream);
    static CommandInform::Ptr parse(ByteStream& stream);
};

struct CommandTrap:CommandInform{
    typedef std::shared_ptr< CommandTrap > Ptr;

    CommandTrap():CommandInform(){
        cmd_type = CommandType::Trap;
    }

    void marshall( ByteStream& stream);
    static CommandTrap::Ptr parse(ByteStream& stream);
};


struct CommandListener{
    typedef std::shared_ptr< CommandListener > Ptr;
    virtual ~CommandListener() {};
    virtual void onCommand(const Command::Ptr& command){};
    virtual void reset(){}
    SecChannel *  channel;

};

///

typedef std::function< std::shared_ptr<Command> (ByteStream& stream) > ParseFuncCommand;
extern std::vector<ParseFuncCommand> parse_func_list_command;
extern std::map< CommandType, ParseFuncCommand> CommandParseFuncList ;

#endif //MONKEY_COMMAND_H
