//
// Created by zhaohaibin on 2/2/18.
//

#include "SmartLog.h"
#include "log4cplus/configurator.h"
#include "log4cplus/appender.h"
#include "log4cplus/fileappender.h"
#include "log4cplus/helpers/pointer.h"

using namespace log4cplus::helpers;

namespace smartlog
{
	log4cplus::Initializer initializer;

	void load_config(const wstring& configfile)
	{
		PropertyConfigurator::doConfigure(configfile);
	}

	void add_log(const wstring& logfile, const wstring& logerName)
    {
		Logger logger = Logger::getInstance(logerName);
		SharedObjectPtr<Appender> spAppender(
			new RollingFileAppender(
				logfile, 100*1024*1024, 5, true, true));
		log4cplus::tstring pattern = LOG4CPLUS_TEXT("[%D{%y-%m-%d %H:%M:%S:%q}] [%p] - %m %F %L%n");
		spAppender->setLayout(std::unique_ptr<Layout>(new PatternLayout(pattern)));
		logger.addAppender(spAppender);
    }
}