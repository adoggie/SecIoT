//
// Created by bin zhang on 2019/1/6.
//

#ifndef _BASE_H
#define _BASE_H

#include <string>
#include <vector>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <thread>


#include <boost/any.hpp>
#include <boost/format.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>


typedef  std::map< std::string , boost::any > PropertyMap;
typedef  std::map< std::string , std::string > PropertyStringMap;

typedef unsigned char * BytePtr;
//typedef std::vector<unsigned char> ByteStream;

class  Object{
public:
	typedef std::shared_ptr<Object> Ptr;
	Ptr data(){
		return data_;
	}

	void data(const Ptr& ptr){
		data_ = ptr;
	}
	Object(){}
private:

protected:
	Ptr data_;
	std::condition_variable cv_;
	std::recursive_mutex rmutex_;
	std::mutex			mutex_;
};

#define SCOPED_LOCK  std::lock_guard<std::mutex> lock(mutex_);

#ifdef _ARM
#define SETTINGS_FILE "/home/smartbox/settings.txt"
#define SETTINGS_USER_FILE "/home/smartbox/settings.user"
#define HOME_PATH "/home/smartbox"
#else
#define SETTINGS_FILE "settings.txt"
#define SETTINGS_USER_FILE "settings.user"
#define HOME_PATH "."
#endif

#endif //_BASE_H
