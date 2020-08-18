//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_REMOTEMAINTENACEIMPL_H
#define MONKEY_REMOTEMAINTENACEIMPL_H

#include "../GM50/command.h"
#include "../GM50/apps/RemoteMonitor.h"

class RemoteMaintenanceImpl:public CommandListener {
public:
    typedef std::shared_ptr<RemoteMaintenanceImpl> Ptr;

    void onCommand(const Command::Ptr& command){}

};


#endif //MONKEY_REMOTEMAINTENACEIMPL_H
