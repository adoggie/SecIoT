//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_COMPLIANCETEST_H
#define MONKEY_COMPLIANCETEST_H


#include "../command.h"

/// GM53/6.2


struct  ComplianceSchemaDetail{
    std::uint8_t  algorithm;
    std::uint8_t  mode;
    std::uint8_t  cipher;
    std::uint8_t  compose;
    ComplianceSchemaDetail(){}
    ComplianceSchemaDetail(std::uint8_t a,std::uint8_t b,std::uint8_t c,std::uint8_t d){
        algorithm = a;
        mode = b;
        cipher= c;
        compose = d;
    }
    bool operator == (const ComplianceSchemaDetail & other){
        if( algorithm == other.algorithm && mode == other.mode && cipher == other.cipher && compose == other.compose){
            return true;
        }
        return false;
    }
};

struct CommandComplianceTestRequest:public    Command{
    typedef std::shared_ptr<CommandComplianceTestRequest> Ptr;
    DeviceID        device_id;
    std::uint8_t    direction; // 0: up , 1 : down
    ComplianceSchemaDetail  schema;
    std::uint32_t   len; //
    ByteArray       data;
public :
    CommandComplianceTestRequest():Command(CommandType::ComplianceTestRequest, AppID::ComplianceTest){

    }

    void marshall( ByteStream& stream);
    static CommandComplianceTestRequest::Ptr parse(ByteStream& stream);
};


struct CommandComplianceTestResponse:public    Command{
    typedef std::shared_ptr<CommandComplianceTestResponse> Ptr;
    DeviceID   device_id;
    std::uint8_t  direction; // 0: up , 1 : down
    std::uint8_t   check;
    ComplianceSchemaDetail  schema;
    std::uint32_t  len_or_data;
    ByteArray     data;
public :
    CommandComplianceTestResponse():Command(CommandType::ComplianceTestResponse, AppID::ComplianceTest){

    }

    void marshall( ByteStream& stream);
    static CommandComplianceTestResponse::Ptr parse(ByteStream& stream);
};


class ComplianceTest {

};


#endif //MONKEY_COMPLIANCETEST_H
