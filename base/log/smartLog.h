//
// Created by zhaohaibin on 2/2/18.
//

#ifndef IMAGEMATCHER_SMARTLOG_H
#define IMAGEMATCHER_SMARTLOG_H

#include <string>
using namespace std;


#include <boost/shared_ptr.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>

using namespace boost;
using namespace boost::log;

BOOST_LOG_ATTRIBUTE_KEYWORD(tag_attr, "Tag", std::string);

namespace smartlog
{
    void add_log(const string& path, const string& fileName, const string& alias);
}

#define LOG_INFO_ALIAS(alias, msg) \
{\
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> slg; \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SCOPED_THREAD_TAG("Tag",alias);\
	BOOST_LOG_SEV(slg, boost::log::trivial::info) <<"[INFO] "<< msg;\
}

#define LOG_WARNING_ALIAS(alias, msg) \
{\
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> slg; \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SCOPED_THREAD_TAG("Tag",alias);\
	BOOST_LOG_SEV(slg, boost::log::trivial::warning) <<"[WARNING] "<< msg;\
}

#define LOG_ERROR_ALIAS(alias, msg) \
{\
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> slg; \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SCOPED_THREAD_TAG("Tag",alias);\
	BOOST_LOG_SEV(slg, boost::log::trivial::error) <<"[ERROR] "<< msg;\
}



#endif //IMAGEMATCHER_SMARTLOG_H
