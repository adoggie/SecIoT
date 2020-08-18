//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_REMOTEMONITORIMPL_H
#define MONKEY_REMOTEMONITORIMPL_H

#include "../GM50/command.h"
#include "../GM50/apps/RemoteMonitor.h"

class RemoteMonitorImpl :public CommandListener{
public:
    typedef std::shared_ptr<RemoteMonitorImpl> Ptr;

    void onCommand(const Command::Ptr& command);

    void onCommandRemoteMonitorRequest(const CommandRemoteMonitorRequest::Ptr& command);
    void onCommandRemoteMonitorResponse(const CommandRemoteMonitorResponse::Ptr& command);
};


#endif //MONKEY_REMOTEMONITORIMPL_H
