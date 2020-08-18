//
// Created by scott on 2020-08-17.
//

#include "include/sparkcow.h"
#include "InProcConnector.h"
#include "Controller.h"
#include "ContextConnection.h"

int sparkcow_init(sparkcow_context_t* ctx){

    auto conn = std::make_shared<ContextConnection>(ctx);
    Controller::instance()->onConnected(conn);
}

void sparkcow_cleanup(sparkcow_context_t* ctx){

}