//
// Created by scott on 8/4/20.
//

#include "SecKeyManagementImpl.h"

#include "../GM50/apps/SecKeyManagement.h"
#include "../sdf-utils.h"
#include "../ManagedAgent.h"
#include "../GM50/sechannel.h"

void SecKeyManagementImpl::onCommand(const Command::Ptr& command){
    {
        CommandSecKeyNegotiateRequest::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyNegotiateRequest>(command);
        if (cp.get()) {
            onCommandSecKeyNegotiateRequest(cp);
            return;
        }
    }
    {
        CommandSecKeyNegotiateResponse::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyNegotiateResponse>(command);
        if (cp.get()) {
            onCommandSecKeyNegotiateResponse(cp);
            return;
        }
    }
    {
        CommandSecKeyDistributeRequest::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyDistributeRequest>(command);
        if (cp.get()) {
            onCommandSecKeyDistributeRequest(cp);
            return;
        }
    }
    {
        CommandSecKeyDistributeResponse::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyDistributeResponse>(command);
        if (cp.get()) {
            onCommandSecKeyDistributeResponse(cp);
            return;
        }
    }

    {
        CommandSecKeyDestroyRequest::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyDestroyRequest>(command);
        if (cp.get()) {
            onCommandSecKeyDestroyRequest(cp);
            return;
        }
    }
    {
        CommandSecKeyDestroyResponse::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyDestroyResponse>(command);
        if (cp.get()) {
            onCommandSecKeyDestroyResponse(cp);
            return;
        }
    }
    {
        CommandSecKeyEnableRequest::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyEnableRequest>(command);
        if (cp.get()) {
            onCommandSecKeyEnableRequest(cp);
            return;
        }
    }
    {
        CommandSecKeyEnableResponse::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyEnableResponse>(command);
        if (cp.get()) {
            onCommandSecKeyEnableResponse(cp);
            return;
        }
    }
    {
        CommandSecKeyApplyRequest::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyApplyRequest>(command);
        if (cp.get()) {
            onCommandSecKeyApplyRequest(cp);
            return;
        }
    }
    {
        CommandSecKeyApplyResponse::Ptr cp = std::dynamic_pointer_cast<CommandSecKeyApplyResponse>(command);
        if (cp.get()) {
            onCommandSecKeyApplyResponse(cp);
            return;
        }
    }
}

void SecKeyManagementImpl::onCommandSecKeyNegotiateRequest(const CommandSecKeyNegotiateRequest::Ptr& command){
    ByteArray  dec_key;
    sdf::data_dec( sdf::KeyID(), command->algorithm_id,NULL,0,command->key_data.data(),command->key_data.size(),dec_key  );
    this->channel->setTempKey(dec_key);

    auto cp = std::make_shared<CommandSecKeyNegotiateResponse>();
    cp->len = 4;
    cp->data = 0 ;

    this->channel->sendCommand(cp);
}

void SecKeyManagementImpl::onCommandSecKeyNegotiateResponse(const CommandSecKeyNegotiateResponse::Ptr& command){

}

void SecKeyManagementImpl::onCommandSecKeyDistributeRequest(const CommandSecKeyDistributeRequest::Ptr& command){
    auto cp = std::make_shared<CommandSecKeyDistributeResponse>();

    for(auto & key : command->dist_Keys){
        ByteArray dec_key;
        sdf::data_dec( sdf::KeyID(),
                       sdf::AlgorithmType(0),
                this->channel->getTempKey().data() ,
                this->channel->getTempKey().size(),
                key.key_set.data(),
                key.key_set.size(),
                dec_key  );
        key.key_set = dec_key;

        cp->dist_keys.push_back( key.key_id );
    }
    sdf::import_keys(command->dist_Keys); /// import into mechine

    // response
    this->channel->sendCommand(cp);
}

void SecKeyManagementImpl::onCommandSecKeyDistributeResponse(const CommandSecKeyDistributeResponse::Ptr& command){
    // do nothing
}

void SecKeyManagementImpl::onCommandSecKeyDestroyRequest(const CommandSecKeyDestroyRequest::Ptr& command){
    sdf::destroy_keys();
    auto cp = std::make_shared<CommandSecKeyDestroyResponse>();
    cp->len = 4;
    cp->result = 0;
    this->channel->sendCommand(cp);
}

void SecKeyManagementImpl::onCommandSecKeyDestroyResponse(const CommandSecKeyDestroyResponse::Ptr& command){
 // do nothing
}

void SecKeyManagementImpl::onCommandSecKeyEnableRequest(const CommandSecKeyEnableRequest::Ptr& command){
    if( command->flag == 1 ){

    }
    sdf::enable_keys( command->key_ids);

    auto cr = std::make_shared<CommandSecKeyEnableResponse>();
    cr->key_ids = command->key_ids;
    this->channel->sendCommand(cr);
}

void SecKeyManagementImpl::onCommandSecKeyEnableResponse(const CommandSecKeyEnableResponse::Ptr& command){
    // do nothing
}

void SecKeyManagementImpl::onCommandSecKeyApplyRequest(const CommandSecKeyApplyRequest::Ptr& command){


}

// retrieve keys from management server
void SecKeyManagementImpl::onCommandSecKeyApplyResponse(const CommandSecKeyApplyResponse::Ptr& command){
    if( command->result!=0 ){
        std::cout << "security key apply failed " << std::endl;
        return;
    }
    for(auto & key : command->key_list){
        ByteArray dec_key;
        sdf::data_dec( sdf::KeyID(),
                       sdf::AlgorithmType(0),
                       this->channel->getTempKey().data() ,
                       this->channel->getTempKey().size(),
                       key.key_set.data(),
                       key.key_set.size(),
                       dec_key  );
        key.key_set = dec_key;
    }
    sdf::import_keys(command->key_list); /// import into mechine

}

void SecKeyManagementImpl::applySecKey(std::uint64_t key_type, std::uint64_t  key_num){
    auto cr = std::make_shared<CommandSecKeyApplyRequest>();
    cr->key_type = key_type;
    cr->key_num = key_num;

    this->channel->sendCommand(cr);

}