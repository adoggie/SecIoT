//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_SECKEYMANAGEMENTIMPL_H
#define MONKEY_SECKEYMANAGEMENTIMPL_H

#include "../GM50/command.h"
#include "../GM50/apps/SecKeyManagement.h"

class SecKeyManagementImpl :public CommandListener{
public:
    typedef std::shared_ptr<SecKeyManagementImpl> Ptr;

    void onCommand(const Command::Ptr& command);
    void onCommandSecKeyNegotiateRequest(const CommandSecKeyNegotiateRequest::Ptr& command);
    void onCommandSecKeyNegotiateResponse(const CommandSecKeyNegotiateResponse::Ptr& command);
    void onCommandSecKeyDistributeRequest(const CommandSecKeyDistributeRequest::Ptr& command);
    void onCommandSecKeyDistributeResponse(const CommandSecKeyDistributeResponse::Ptr& command);
    void onCommandSecKeyDestroyRequest(const CommandSecKeyDestroyRequest::Ptr& command);
    void onCommandSecKeyDestroyResponse(const CommandSecKeyDestroyResponse::Ptr& command);
    void onCommandSecKeyEnableRequest(const CommandSecKeyEnableRequest::Ptr& command);
    void onCommandSecKeyEnableResponse(const CommandSecKeyEnableResponse::Ptr& command);
    void onCommandSecKeyApplyRequest(const CommandSecKeyApplyRequest::Ptr& command);
    void onCommandSecKeyApplyResponse(const CommandSecKeyApplyResponse::Ptr& command);

    void applySecKey(std::uint64_t key_type, std::uint64_t  key_num);
};


#endif //MONKEY_SECKEYMANAGEMENTIMPL_H
