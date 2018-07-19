/*
    This file is part of cpp-ethereum.

    cpp-ethereum is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    cpp-ethereum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with cpp-ethereum.  If not, see <http://www.gnu.org/licenses/>.
*/
/** @file Log.cpp
 * @author Gav Wood <i@gavwood.com>
 * @date 2014
 */

#include "Log.h"

#ifdef __APPLE__
#include <pthread.h>
#endif

// #include <iostream>
// #include <boost/date_time/posix_time/posix_time.hpp>
// #include <boost/core/null_deleter.hpp>
// #include <boost/log/attributes/clock.hpp>
// #include <boost/log/attributes/function.hpp>
// #include <boost/log/expressions.hpp>
// #include <boost/log/sinks/text_ostream_backend.hpp>
// #include <boost/log/sources/global_logger_storage.hpp>
// #include <boost/log/sources/severity_channel_logger.hpp>
// #include <boost/log/support/date_time.hpp>
#include <boost/log/utility/exception_handler.hpp>
// #include <boost/log/utility/setup/common_attributes.hpp>
// #include <boost/log/utility/setup/console.hpp>
// #include <boost/log/common.hpp>
// #include <boost/log/expressions/keyword.hpp>
// #include <boost/log/sources/logger.hpp>
// #include <boost/log/attributes.hpp>
// #include <boost/log/attributes/timer.hpp>
// #include <boost/log/attributes/named_scope.hpp>
// #include <boost/log/utility/setup/file.hpp>
// #include <boost/log/sinks/text_file_backend.hpp>
// #include <boost/log/sinks/sync_frontend.hpp>


// #define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
// #undef BOOST_NO_CXX11_SCOPED_ENUMS

#include <iostream>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/common.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/sinks/sync_frontend.hpp>
#include <boost/log/sinks/text_file_backend.hpp>

#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

#include <boost/log/attributes/named_scope.hpp>

template <class T>
using log_sink = boost::log::sinks::synchronous_sink<T>;

namespace logging = boost::log;
namespace attrs = boost::log::attributes;
namespace src = boost::log::sources;
namespace sinks = boost::log::sinks;
namespace expr = boost::log::expressions;
namespace keywords = boost::log::keywords;

namespace dev
{
namespace
{
/// Associate a name with each thread for nice logging.
struct ThreadLocalLogName
{
    ThreadLocalLogName(std::string const &_name) { m_name.reset(new std::string(_name)); }
    boost::thread_specific_ptr<std::string> m_name;
};

ThreadLocalLogName g_logThreadName("main");
} // namespace

std::string getThreadName()
{
#if defined(__GLIBC__) || defined(__APPLE__)
    char buffer[128];
    pthread_getname_np(pthread_self(), buffer, 127);
    buffer[127] = 0;
    return buffer;
#else
    return g_logThreadName.m_name.get() ? *g_logThreadName.m_name.get() : "<unknown>";
#endif
}

void setThreadName(std::string const &_n)
{
#if defined(__GLIBC__)
    pthread_setname_np(pthread_self(), _n.c_str());
#elif defined(__APPLE__)
    pthread_setname_np(_n.c_str());
#else
    g_logThreadName.m_name.reset(new std::string(_n));
#endif
}

BOOST_LOG_ATTRIBUTE_KEYWORD(channel, "Channel", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(context, "Context", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(threadName, "ThreadName", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_severity, "Severity", dev::Verbosity)
BOOST_LOG_ATTRIBUTE_KEYWORD(timestamp, "TimeStamp", boost::posix_time::ptime)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_uptime, "Uptime", attrs::timer::value_type)
BOOST_LOG_ATTRIBUTE_KEYWORD(log_scope, "Scope", attrs::named_scope::value_type)

void g_InitLog();

void setupLogging(LoggingOptions const &_options)
{
    auto sink = boost::make_shared<log_sink<boost::log::sinks::text_ostream_backend>>();

    boost::shared_ptr<std::ostream> stream{&std::cout, boost::null_deleter{}};
    sink->locked_backend()->add_stream(stream);
    sink->set_filter([_options](boost::log::attribute_value_set const &_set) {
        if (_set["Severity"].extract<int>() > _options.verbosity)
            return false;

        auto const messageChannel = _set[channel];
        return (_options.includeChannels.empty() ||
                contains(_options.includeChannels, messageChannel)) &&
               !contains(_options.excludeChannels, messageChannel);
    });

    namespace expr = boost::log::expressions;
    sink->set_formatter(expr::stream
                        << EthViolet << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S")
                        << EthReset " " EthNavy << threadName << EthReset " " << channel
                        << expr::if_(expr::has_attr(
                               context))[expr::stream << " " EthNavy << context << EthReset]
                        << " " << expr::smessage);

    boost::log::core::get()->add_sink(sink);

    boost::log::core::get()->add_global_attribute(
        "ThreadName", boost::log::attributes::make_function(&getThreadName));
    boost::log::core::get()->add_global_attribute(
        "TimeStamp", boost::log::attributes::local_clock());

    boost::log::core::get()->set_exception_handler(
        boost::log::make_exception_handler<std::exception>([](std::exception const &_ex) {
            std::cerr << "Exception from the logging library: " << _ex.what() << '\n';
        }));

    //

    g_InitLog();
}

void g_InitLog()
{
    logging::formatter formatter =
        expr::stream
        << "[" << expr::format_date_time(timestamp, "%H:%M:%S")
        << "]" << expr::if_(expr::has_attr(log_uptime))[expr::stream << " [" << format_date_time(timestamp, "%O:%M:%S") << "]"]

        << expr::if_(expr::has_attr(log_scope))
               [expr::stream << "[" << expr::format_named_scope(log_scope, keywords::format = "%n") << "]"]
        << "<" << log_severity << ">" << expr::message;

    logging::add_common_attributes();

    auto console_sink = logging::add_console_log();
    auto file_sink = logging::add_file_log(
        keywords::file_name = "%Y-%m-%d_%N.log",                                     //文件名
        keywords::rotation_size = 10 * 1024 * 1024,                                  //单个文件限制大小
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0) //每天重建
    );

    file_sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = "logs",                   //文件夹名
        keywords::max_size = 50 * 1024 * 1024,       //文件夹所占最大空间
        keywords::min_free_space = 100 * 1024 * 1024 //磁盘最小预留空间
        ));

    file_sink->set_filter(log_severity >= VerbosityWarning); //日志级别过滤

    file_sink->locked_backend()->scan_for_files();

    console_sink->set_formatter(formatter);
    file_sink->set_formatter(formatter);
    file_sink->locked_backend()->auto_flush(true);

    logging::core::get()->add_global_attribute("Scope", attrs::named_scope());
    logging::core::get()->add_sink(console_sink);
    logging::core::get()->add_sink(file_sink);
}

} // namespace dev
