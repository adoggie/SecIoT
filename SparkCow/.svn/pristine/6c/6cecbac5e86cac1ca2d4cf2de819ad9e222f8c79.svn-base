//
// Created by scott on 8/4/20.
//

#ifndef MONKEY_MANAGEDAGENT_H
#define MONKEY_MANAGEDAGENT_H

#include "base.h"
#include "config.h"
//#include "GM50/base.h"
#include "bundles/all.h"
#include "UserSecChannel.h"

class ManagedAgent : public IConnectionListener,public std::enable_shared_from_this<ManagedAgent>{
DECL_SINGLETON(ManagedAgent)
public:
    struct Settings{
        ByteArray tmp_key;
        struct {
            std::uint64_t KeyStatus;
            std::uint64_t KeyUpdateStatus;
            std::uint64_t KeyTunnelStatus;
            std::uint64_t KeyLostMonitor;
            std::uint64_t KeyIPChanged;
            std::uint64_t RandGeneratorStatus;
            std::uint64_t KeyProgramCheck;
            std::uint64_t LogNum;
        } vid_monitor;
    };
    bool init(const Config& cfgs);
    bool open();
    void close();
    Settings&  getSettings(){ return settings_;}

//    SecChannel& getSecChannel(){ return channel_;}
    void onConnected(const Connection::Ptr& conn);
private:
    Settings  settings_;
//    UserSecChannel  channel_;
    Config cfgs_;
};


#endif //MONKEY_MANAGEDAGENT_H
