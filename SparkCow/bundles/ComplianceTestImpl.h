//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_COMPLIANCETESTIMPL_H
#define MONKEY_COMPLIANCETESTIMPL_H
#include <memory>

#include "../GM50/command.h"
#include "../GM50/apps/ComplianceTest.h"

class ComplianceTestImpl :public CommandListener {
public:
    typedef std::shared_ptr<ComplianceTestImpl> Ptr;

    void onCommand(const Command::Ptr& command);

    void onCommandComplianceTestRequest(const CommandComplianceTestRequest::Ptr& command);
    void onCommandComplianceTestResponse(const CommandComplianceTestResponse::Ptr& command);
private:
    void onSymmetricEncrypt(const ComplianceSchemaDetail& schema, std::uint32_t algo_id,const ByteArray & data); // plain -> cipher_text and cipher
    void onSymmetricEncrypt2(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & plain, const ByteArray& cipher); // plain and cipher -> cipher text

    void onSymmetricDecrypt(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & cipher,const ByteArray & cipher_text);

    void onASymmetricEncrypt(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray& pub_key,const ByteArray & data);
    void onASymmetricDecrypt(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & priv_key,const ByteArray & envelope);

    void onASymmetricSign(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray& priv_key,const ByteArray & data);
    void onASymmetricVerify(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,const ByteArray & pub_key,const ByteArray & sign_val);
    // 6.2.3.1.3  e)
    // pubkey and temp pubkey and sender id ->  pubkey , temp pubkey and response id
    void onExchangeKey(const ComplianceSchemaDetail& schema,std::uint32_t algo_id, const ByteArray& pub_key, const ByteArray& temp_pub_key,std::uint32_t sender);
    void onHashCheck(const ComplianceSchemaDetail& schema,std::uint32_t algo_id,ByteArray& data);

    void onDeviceSelfCheck(std::uint32_t content);
};


#endif //MONKEY_COMPLIANCETESTIMPL_H
