//
// Created by scott on 2020-08-17.
//

#include "InProcDirectionIn.h"
#include "SDKDeviceManagementImpl.h"


bool InProcDirectionIn::init(const Config& props){
    auto cfgs = props;
    auto conn = std::dynamic_pointer_cast<ContextConnection>(conn_);
    auto ep_cfgs = conn->getContext()->on_get_endpoint_cfgs();

    cfgs.set_string("remote_pub_key",std::string(ep_cfgs->remote_public_key->data,
            ep_cfgs->remote_public_key->data+ ep_cfgs->remote_public_key->len));

    cfgs.set_string("local_pri_key",std::string(ep_cfgs->local_private_key->data,
                                                ep_cfgs->local_private_key->data+ ep_cfgs->local_private_key->len));
    cfgs.set_string("local_pub_key",std::string(ep_cfgs->local_public_key->data,
                                                 ep_cfgs->local_public_key->data+ ep_cfgs->local_public_key->len));
    cfgs.set_string("remote_id",std::string(ep_cfgs->remote_device_id->data,
                                             ep_cfgs->remote_device_id->data+ ep_cfgs->remote_device_id->len));
    cfgs.set_string("local_id",std::string(ep_cfgs->local_device_id->data,
                                            ep_cfgs->local_device_id->data+ ep_cfgs->local_device_id->len));
    SecChannel::init(cfgs);
    return true;
}

void InProcDirectionIn::initCommandListener(){
    SecChannel::initCommandListener();

    CommandListener::Ptr user = std::make_shared< SDKDeviceManagementImpl >( shared_from_this());
    setCommandListener( AppID::DeviceManagement, user );

}