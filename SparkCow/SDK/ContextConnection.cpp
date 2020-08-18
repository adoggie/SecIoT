//
// Created by scott on 2020-08-17.
//

#include "ContextConnection.h"
#include "Controller.h"
#include "include/sparkcow.h"

ContextConnection::ContextConnection(sparkcow_context_t * ctx):Connection(Controller::instance()->get_io_service()){
    context_ = ctx;
}

void ContextConnection::send(const char * data,size_t size){
    auto data_ = context_->data_new(data,(uint32_t)size);
    context_->on_data(data_);
}