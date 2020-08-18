//
// Created by scott on 2020-08-17.
//

#ifndef SMARTBOX_INPROCCONNECTOR_H
#define SMARTBOX_INPROCCONNECTOR_H

#include "../Connector.h"

struct sparkcow_context_t;

class InProcConnector :public Connector{

public:
    typedef std::shared_ptr<InProcConnector> Ptr;
    InProcConnector() {
//        context_ = ctx;
    }
    void incoming(const Connection::Ptr& conn);
    void onLostChannel(const SecChannel::Ptr& peer);

protected:
    sparkcow_context_t*  context_;
};


#endif //SMARTBOX_INPROCCONNECTOR_H
