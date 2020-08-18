//
// Created by scott on 2020-08-17.
//

#include "SDKDeviceManagementImpl.h"
#include "include/sparkcow.h"
#include "../GM50/attribute.h"
#include "InProcDirectionIn.h"

std::map<uint64_t ,sparkcow_attr_type_t> attribute_maps={
        {AID_Vendor,attr_basic_vendor},

};

void SDKDeviceManagementImpl::onCommandGet(const CommandGet::Ptr& command){
    //查询设备信息
    auto cp =  std::make_shared<CommandResponse>();

    for(auto attr_id: command->aids){
        auto detail = AttributeTable::get(attr_id);
        auto itr = attribute_maps.find(attr_id);
        if(itr == attribute_maps.end()){
            continue;
        }
        //回调用户返回配置阐述
        auto conn = direction_->getConnection();
        auto value = conn->getContext()->management.dev_mgt_get_attr(itr->second,0);
        auto attr = std::make_shared<Attribute >();
        attr->aid = attr_id;
        if(detail->type == AttributeValueType::String){
            attr->value = std::string(value->val.str.data,value->val.str.data+value->val.str.len);
        }
        cp->attrs.push_back(attr);
        conn->getContext()->management.attr_free(value);
    }
    auto peer = direction_->against();
    if(peer){
        peer->sendCommand(cp);
    }
}

void SDKDeviceManagementImpl::onCommandGetNext(const CommandGetNext::Ptr& command){

}

void SDKDeviceManagementImpl::onCommandGetBulk(const CommandGetBulk::Ptr& command){


}


void SDKDeviceManagementImpl::onCommandResponse(const CommandResponse::Ptr& command){

}


void SDKDeviceManagementImpl::onCommandSet(const CommandSet::Ptr& command){


}


void SDKDeviceManagementImpl::onCommandInform(const CommandInform::Ptr& command){


}


void SDKDeviceManagementImpl::onCommandTrap(const CommandTrap::Ptr& command){

}

