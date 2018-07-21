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

#include <boost/log/support/date_time.hpp>

#include <boost/core/null_deleter.hpp>
#include <boost/log/attributes/clock.hpp>
#include <boost/log/attributes/function.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/severity_channel_logger.hpp>
#include <boost/log/utility/exception_handler.hpp>


#include <boost/log/common.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/expressions/keyword.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/attributes/timer.hpp>
#include <boost/log/attributes.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/detail/config.hpp>


#if defined(NDEBUG)
#include <boost/log/sinks/async_frontend.hpp>
template <class T>
using log_sink = boost::log::sinks::asynchronous_sink<T>;
#else
#include <boost/log/sinks/sync_frontend.hpp>
template <class T>
using log_sink = boost::log::sinks::synchronous_sink<T>;
#endif

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
    ThreadLocalLogName(std::string const& _name) { m_name.reset(new std::string(_name)); }
    boost::thread_specific_ptr<std::string> m_name;
};

ThreadLocalLogName g_logThreadName("main");
}  // namespace

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

void setThreadName(std::string const& _n)
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
void g_InitLog(logging::formatter formatter);
void setupLogging(LoggingOptions const& _options)
{
    auto sink = boost::make_shared<log_sink<sinks::text_ostream_backend>>();

    boost::shared_ptr<std::ostream> stream{&std::cout, boost::null_deleter{}};
    sink->locked_backend()->add_stream(stream);
    sink->set_filter([_options](logging::attribute_value_set const& _set) {
        if (_set["Severity"].extract<int>() > _options.verbosity)
            return false;

        auto const messageChannel = _set[channel];
        return (_options.includeChannels.empty() ||
                   contains(_options.includeChannels, messageChannel)) &&
               !contains(_options.excludeChannels, messageChannel);
    });

    logging::formatter formatter = expr::stream
                        << EthViolet "[" << expr::format_date_time(timestamp, "%Y-%m-%d %H:%M:%S")
                        <<"]" EthReset "" EthNavy "[" << threadName << "]" EthReset "<" << channel << ">"
                        << expr::if_(expr::has_attr(
                               context))[expr::stream << " " EthNavy << context << EthReset]
                        << " " << expr::smessage;

    sink->set_formatter(formatter);

    logging::core::get()->add_sink(sink);

    logging::core::get()->add_global_attribute(
        "ThreadName", logging::attributes::make_function(&getThreadName));
    logging::core::get()->add_global_attribute(
        "TimeStamp", logging::attributes::local_clock());

    logging::core::get()->set_exception_handler(
        logging::make_exception_handler<std::exception>([](std::exception const& _ex) {
        std::cerr << "Exception from the logging library: " << _ex.what() << '\n';
    }));
     g_InitLog(formatter);
}

void g_InitLog(logging::formatter formatter)
{
    logging::add_common_attributes();
    auto file_sink = logging::add_file_log(
        keywords::file_name = "%Y-%m-%d_%N.log",                                     //文件名
        keywords::rotation_size = 10 * 1024 * 1024,                                  //单个文件限制大小
        keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0) //每天重建
    );

    file_sink->locked_backend()->set_file_collector(sinks::file::make_collector(
        keywords::target = "TOSLOG",                   //文件夹名
        keywords::max_size = 50 * 1024 * 1024,       //文件夹所占最大空间
        keywords::min_free_space = 100 * 1024 * 1024 //磁盘最小预留空间
        ));

    // file_sink->set_filter(log_severity >= VerbosityWarning); //日志级别过滤
    file_sink->locked_backend()->scan_for_files();
    file_sink->set_formatter(formatter);
    file_sink->locked_backend()->auto_flush(true);
    logging::core::get()->add_sink(file_sink);
}

} 