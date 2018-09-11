//
// Created by zhaohaibin on 2/2/18.
//

#ifndef IMAGEMATCHER_SMARTLOG_H
#define IMAGEMATCHER_SMARTLOG_H

#include <string>
using namespace std;

#include <boost/log/sinks/text_ostream_backend.hpp>
// #include <boost/log/attributes/named_scope.hpp>
// #include <boost/log/attributes/scoped_attribute.hpp>

#include "log4cplus/initializer.h"
#include <iomanip>
#include "log4cplus/loggingmacros.h"
#include "log4cplus/tstring.h"

using namespace boost;
using namespace boost::log;
using namespace log4cplus;
//BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string);

namespace smartlog
{
	void load_config(const wstring& configfile);
    void add_log(const wstring& logfile, const wstring& logerName);
}

#define LOG_INFO_ALIAS(alias, msg) \
{\
	LOG4CPLUS_INFO(Logger::getInstance(LOG4CPLUS_TEXT(alias)), msg);\
}

#define LOG_WARNING_ALIAS(alias, msg) \
{\
	LOG4CPLUS_WARN(Logger::getInstance(LOG4CPLUS_TEXT(alias)), msg);\
}

#define LOG_ERROR_ALIAS(alias, msg) \
{\
	LOG4CPLUS_ERROR(Logger::getInstance(LOG4CPLUS_TEXT(alias)), msg);\
}

#endif //IMAGEMATCHER_SMARTLOG_H
