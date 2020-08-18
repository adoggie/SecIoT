//
// Created by scott on 2020-08-17.
//

#include "InProcConnector.h"
#include "InProcDirectionIn.h"

void InProcConnector::incoming(const Connection::Ptr& conn){
//    Application::instance()->getLogger().debug("New  Connection Established!");
    auto p = std::dynamic_pointer_cast<Connector>(shared_from_this());
    auto in = std::make_shared<InProcDirectionIn>(p);
    in->init(cfgs_);
    in->open(conn);
    in_ = in;
}

void InProcConnector::onLostChannel(const SecChannel::Ptr& peer){

}