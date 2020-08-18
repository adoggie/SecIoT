//
// Created by bin zhang on 2019/1/6.
//

#ifndef INNERPROC_APP_H
#define INNERPROC_APP_H

#include "base.h"
#include "service.h"
#include "config.h"

#include "logger.h"

#include <boost/asio.hpp>

class Application;

struct IController{
    virtual bool init(const Config& cfgs) { return true;} ;
    virtual void run(){};
    virtual bool open(){ return true;};
    virtual void close(){};

    boost::asio::io_service& get_io_service(){
        static boost::asio::io_service io_service_;
        return io_service_;
    }
};

class Application:public Object,public ServiceContainer{
	Logger 	logger_;
	Config  cfgs_;
	std::vector<std::shared_ptr<Service> > services_;
public:
	static std::shared_ptr<Application>& instance();
	Application&  init(IController * controller);
	Logger& getLogger();
	void run();
	void stop();
	void addService(std::shared_ptr<Service>& service );
	Config& getConfig();
	std::string name();
    IController* getController(){ return controller_;}
protected:
	void wait_for_shutdown();

    IController * controller_;
};

#endif
