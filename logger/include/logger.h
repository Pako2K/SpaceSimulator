#ifndef LOGGER_H
#define LOGGER_H

/*
  BUILD_LOGGER should only be used when compiling the Logger itself

  ENABLE_LOGGER should be used to enable the logger functionality in libraries and executables
                Recommendation: DO NOT USE for Release Builds, but only for Debug and Debug-Lib Builds
*/

#if defined ENABLE_LOGGER | defined BUILD_LOGGER

  #include <functional>
  #include <memory>
  #include <iostream>
  #include <fstream>
  #include <string>
  #include <sstream>
  

  #define InitializeLogger(sink)                  utils::Logger::initialize(sink)
  #define InitializeLoggerLevel(min_level, sink)  utils::Logger::initialize(sink, min_level)

  #define DebugLog(trace)   utils::Logger::getBuffer() << trace; utils::Logger::write(utils::LogLevel::DEBUG)
  #define InfoLog(trace)    utils::Logger::getBuffer() << trace; utils::Logger::write(utils::LogLevel::INFO)
  #define ErrorLog(trace)   utils::Logger::getBuffer() << trace; utils::Logger::write(utils::LogLevel::ERROR)

  #define DestroyLogger()   utils::Logger::destroy()


namespace utils
{
  enum LogLevel : uint8_t { DEBUG, INFO, ERROR };

  class Logger
  {
  public:

    static void initialize(std::ostream& out_stream, LogLevel level = DEBUG);
    static void initialize(const std::string& log_file, LogLevel level = DEBUG);
 
    static std::ostream& getBuffer() { return _buffer; }

    static void write(LogLevel level) { 
      _add(level, _buffer.str());
      _buffer.str("");
    }
 
    static void destroy();

    ~Logger();
    

  private:
    inline static const std::string HEADERS[]{ "DEBUG: ", "INFO: ", "\n***********************************************************************\nERROR: " };
    inline static const std::string TAILS[]{ "\n", "\n", "\n***********************************************************************\n\n" };

    inline static std::unique_ptr<Logger> _logger{ nullptr };
    inline static std::stringstream _buffer;
    inline static std::function<void(LogLevel, const std::string&)> _add{ [](LogLevel, const std::string&) {} };
        
    LogLevel  _min_level{ DEBUG };
    std::ostream& _stream;

    Logger(std::ostream& out_stream, LogLevel min_level) : _min_level{ min_level }, _stream{ out_stream } {}
  };
} // End namespace

#else
 
  #define InitializeLogger(sink)
  #define InitializeLoggerLevel(level, sink)

  #define DebugLog(trace)
  #define InfoLog(trace)
  #define ErrorLog(trace)

  #define DestroyLogger()

#endif 


#endif // LOGGER_H
