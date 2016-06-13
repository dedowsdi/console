#ifndef PACLOGGER_H
#define PACLOGGER_H

#include "pacSingleton.h"

namespace pac {

enum SeverityLevel { SL_TRIVIAL, SL_NORMAL, SL_WARNING, SL_ERROR, SL_CRITICAL };

class _PacExport Logger : public Singleton<Logger> {
public:
  /**
   * ctor
   * @param logFileName : log file name
   */
  Logger(const std::string& logFileName = "console.log");
  ~Logger();

  /**
   * log message in specific servity level
   * @param msg : msg string
   * @param lvl : severity level
   */
  void logMessage(const std::string& msg, SeverityLevel lvl = SL_NORMAL);

  void setSeverityLevel(SeverityLevel lvl);

  void flush();

private:
};
}

#endif /* PACLOGGER_H */
