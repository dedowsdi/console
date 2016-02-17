#include "pacStable.h"
#include "pacLogger.h"
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/support/date_time.hpp>

namespace logging = boost::log;
namespace src = boost::log::sources;
namespace expr = boost::log::expressions;
namespace sinks = boost::log::sinks;
namespace keywords = boost::log::keywords;

namespace pac {

BOOST_LOG_ATTRIBUTE_KEYWORD(severity, "Severity", SeverityLevel)

// The operator is used when putting the severity level to log
logging::formatting_ostream& operator<<(logging::formatting_ostream& strm,
    logging::to_log_manip<SeverityLevel, tag::severity> const& manip) {
  static const char* strings[] = {"TRIV", "NORM", "WARN", "ERRR", "CRIT"};

  SeverityLevel level = manip.get();
  if (static_cast<std::size_t>(level) < sizeof(strings) / sizeof(*strings))
    strm << strings[level];
  else
    strm << static_cast<int>(level);

  return strm;
}

template <>
Logger* Singleton<Logger>::msSingleton = 0;

//------------------------------------------------------------------
Logger::Logger(const std::string& logFileName /*= "console.log"*/) {
  auto sink = logging::add_file_log(keywords::file_name = logFileName,
      // [H:M:S]<TYPE> msg
      keywords::format = expr::stream
                         << "["
                         << expr::format_date_time<boost::posix_time::ptime>(
                                "TimeStamp", "%H:%M:%S") << "]<" << severity
                         << "> " << expr::smessage);
  sink->locked_backend()->auto_flush(true);
  logging::add_common_attributes();
  setSeverityLevel(SL_TRIVIAL);
}

//------------------------------------------------------------------
Logger::~Logger() {
  // logging::core::get()->flush();
}

//------------------------------------------------------------------
void Logger::logMessage(
    const std::string& msg, SeverityLevel lvl /*= SL_NORMAL*/) {
  static src::severity_logger<SeverityLevel> lg;
  BOOST_LOG_SEV(lg, lvl) << msg;
}

//------------------------------------------------------------------
void Logger::setSeverityLevel(SeverityLevel lvl) {
  logging::core::get()->set_filter(severity >= lvl);
}

//------------------------------------------------------------------
void Logger::flush() { logging::core::get()->flush(); }
}
