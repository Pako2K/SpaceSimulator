#include <logger.h>

#include <typeinfo>
#include <exception>


using namespace utils;


void Logger::initialize(std::ostream& out_stream, LogLevel min_level) {
  if (_logger.get())
    throw std::runtime_error("Logger already initialized");

  _logger.reset(new Logger(out_stream, min_level));

  if (min_level > DEBUG)
    _add = [](LogLevel level, const std::string & trace) {
      if (level >= _logger->_min_level) {
        _logger->_stream << HEADERS[level] << trace << TAILS[level];
        _logger->_stream.flush();
      }
    };
  else
    _add = [](LogLevel level, const std::string & trace) {
      _logger->_stream << HEADERS[level] << trace << TAILS[level];
      _logger->_stream.flush();
    };

  _add(DEBUG, "Logger INITIALIZED");
}


void Logger::initialize(const std::string& log_file_name, LogLevel min_level) {
  std::ofstream* log_file = new std::ofstream(log_file_name, std::ios::ate | std::ios::app);
  if (!log_file->is_open()) {
    delete log_file;
    throw std::runtime_error("Log File cannot be opened: " + log_file_name);
  }

  try {
    initialize(*log_file, min_level);
  }
  catch (std::runtime_error&) {
    log_file->close();
    delete log_file;
  }
}


void Logger::destroy() {
  _add(DEBUG, "Logger DESTROYED\n\n");
  _logger.reset(nullptr);
  _add = [](LogLevel, const std::string&) {};
}


Logger::~Logger() {
  // Close and delete file pointer
  if (typeid(_stream) == typeid(std::ofstream)) {
    dynamic_cast<std::ofstream&>(_stream).close();
    delete &_stream;
  }
}

