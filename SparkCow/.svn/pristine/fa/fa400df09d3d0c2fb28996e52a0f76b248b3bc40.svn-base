//
// Created by scott on 2020-08-17.
//

#ifndef SMARTBOX_CONTEXTCONNECTION_H
#define SMARTBOX_CONTEXTCONNECTION_H

#include "../connection.h"
//#include "Controller.h"
#include "include/sparkcow.h"

class ContextConnection:public Connection {
public:
    typedef std::shared_ptr<ContextConnection> Ptr;


    ContextConnection(sparkcow_context_t * ctx);

    sparkcow_context_t* getContext(){
        return context_;
    }

    void send(const char * data,size_t size);
protected:
    sparkcow_context_t  *context_;
};


#endif //SMARTBOX_CONTEXTCONNECTION_H
