//
// Created by zhaohaibin on 2/2/18.
//

#include "SmartLog.h"

namespace smartlog
{
    void add_log(const string& path, const string& fileName, const string& alias)
    {
        boost::log::add_common_attributes();
        boost::log::core::get()->add_global_attribute("Scope",
                                                      boost::log::attributes::named_scope());
        boost::log::core::get()->set_filter(
                boost::log::trivial::severity >= boost::log::trivial::trace
        );

        auto fmtTimeStamp = boost::log::expressions::
        format_date_time<boost::posix_time::ptime>("TimeStamp", "%Y-%m-%d %H:%M:%S.%f");
        auto fmtThreadId = boost::log::expressions::
        attr<boost::log::attributes::current_thread_id::value_type>("ThreadID");
        auto fmtSeverity = boost::log::expressions::
        attr<boost::log::trivial::severity_level>("Severity");
        auto fmtScope = boost::log::expressions::format_named_scope("Scope",
                                                                    boost::log::keywords::format = "%n(%f:%l)",
                                                                    boost::log::keywords::iteration = boost::log::expressions::reverse,
                                                                    boost::log::keywords::depth = 2);

		boost::log::formatter logFmt = boost::log::expressions::stream
			<< "[" << fmtTimeStamp << "]"
			<< "[" << fmtThreadId << "]"
			<< boost::log::expressions::if_(boost::log::expressions::has_attr(tag_attr))
			[
				boost::log::expressions::stream << "[" << tag_attr << "] "
			]
		    << boost::log::expressions::smessage
			<< "[" << fmtScope << "]";

        /* console sink */
        //auto consoleSink = boost::log::add_console_log(std::clog);
        //consoleSink->set_formatter(logFmt);

        /* fs sink */
        boost::filesystem::path logFilePath(path);
        logFilePath.append(fileName);
		unsigned int max_size = (unsigned int)3 * 1024 * 1024 * 1024;
		auto fsSink = boost::log::add_file_log(
			boost::log::keywords::file_name = logFilePath.generic_string(),
			boost::log::keywords::rotation_size = 100 * 1024 * 1024,
			boost::log::keywords::min_free_space = 100 * 1024 * 1024,   //最低磁盘空间限制
			boost::log::keywords::max_size = max_size,							 //所有日志占用的最大空间
                boost::log::keywords::max_files = 12,
                boost::log::keywords::scan_method = boost::log::sinks::file::scan_all,
                boost::log::keywords::target = logFilePath.parent_path().generic_string(),
                boost::log::keywords::open_mode = std::ios_base::app,
				boost::log::keywords::auto_flush = true);
        fsSink->set_formatter(logFmt);
        //fsSink->locked_backend()->auto_flush(true);
		fsSink->set_filter((boost::log::expressions::has_attr(tag_attr) && tag_attr == alias));
		boost::log::core::get()->add_sink(fsSink);

		boost::log::add_common_attributes();
    }
}