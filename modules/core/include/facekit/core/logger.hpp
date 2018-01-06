/**
 *  @file   logger.hpp
 *  @brief  Logging tool
 *          Based on : http://stackoverflow.com/questions/19415845/
 *  @ingroup    core
 *  @author Christophe Ecabert
 *  @date   19/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */
#ifndef __FACEKIT_LOGGER__
#define __FACEKIT_LOGGER__

#include <iostream>
#include <utility>

#include "facekit/core/library_export.hpp"

#ifndef WIN32
/** Reset coloring command */
#define LOG_RESET   "\033[0m"
/** Black coloring command */
#define LOG_BLACK   "\033[30m"
/** Red coloring command */
#define LOG_RED     "\033[31m"
/** Green coloring command */
#define LOG_GREEN   "\033[32m"
/** Yellow coloring command */
#define LOG_YELLOW  "\033[33m"
/** Blue coloring command */
#define LOG_BLUE    "\033[34m"
/** Magenta coloring command */
#define LOG_MAGENTA "\033[35m"
/** Cyan coloring command */
#define LOG_CYAN    "\033[36m"
/** Gray coloring command */
#define LOG_GRAY   "\033[37m"
#else
#define LOG_RESET
#define LOG_BLACK
#define LOG_RED
#define LOG_GREEN
#define LOG_YELLOW
#define LOG_BLUE
#define LOG_MAGENTA
#define LOG_CYAN
#define LOG_GRAY
#endif

/** Error Log */
#define FACEKIT_LOG_ERROR(msg) \
(FaceKit::Logger::Instance().Log(FaceKit::Logger::Level::kError, __FILE__, __LINE__,\
FaceKit::Logger::LogData<FaceKit::Logger::None>() << msg))
/** Warning Log */
#define FACEKIT_LOG_WARNING(msg) \
(FaceKit::Logger::Instance().Log(FaceKit::Logger::Level::kWarning, __FILE__, \
 __LINE__, FaceKit::Logger::LogData<FaceKit::Logger::None>() << msg))
/** Info Log */
#define FACEKIT_LOG_INFO(msg) \
(FaceKit::Logger::Instance().Log(FaceKit::Logger::Level::kInfo, __FILE__, __LINE__, \
 FaceKit::Logger::LogData<FaceKit::Logger::None>() << msg))
/** Debug Log */
#define FACEKIT_LOG_DEBUG(msg) \
(FaceKit::Logger::Instance().Log(FaceKit::Logger::Level::kDebug, __FILE__, __LINE__,\
 FaceKit::Logger::LogData<FaceKit::Logger::None>() << msg))
/** Debug level 1 Log */
#define FACEKIT_LOG_DEBUG1(msg) \
(FaceKit::Logger::Instance().Log(FaceKit::Logger::Level::kDebug1, __FILE__, __LINE__,\
 FaceKit::Logger::LogData<FaceKit::Logger::None>() << msg))
/** Debug level 2 Log */
#define FACEKIT_LOG_DEBUG2(msg) \
(FaceKit::Logger::Instance().Log(FaceKit::Logger::Level::kDebug2, __FILE__, __LINE__,\
 FaceKit::Logger::LogData<FaceKit::Logger::None>() << msg))


/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {

/**
 * @class   Logger
 * @brief   Logging tool
 *          Based on : http://stackoverflow.com/questions/19415845/
 * @ingroup core
 * @author  Christophe Ecabert
 * @date    19/08/2017
 */
class FK_EXPORTS Logger {
 public:

#pragma mark -
#pragma mark Type definition

  /**
   * @enum  Level
   * @brief List of possible level of logging
   */
  enum Level {
    /** Error level */
    kError,
    /** Warning level */
    kWarning,
    /** Info level */
    kInfo,
    /** Debug level */
    kDebug,
    /** Debug level 1 */
    kDebug1,
    /** Debug level 2 */
    kDebug2
  };

  /** Conversion from Level to string */
  static constexpr const char* LvlToStr[]  = {LOG_RED "ERROR" LOG_RESET,
                                              LOG_YELLOW "WARNING" LOG_RESET,
                                              LOG_GREEN "INFO" LOG_RESET,
                                              LOG_GRAY "DEBUG" LOG_RESET,
                                              LOG_GRAY "DEBUG1" LOG_RESET,
                                              LOG_GRAY "DEBUG2" LOG_RESET};

  /**
   * @struct    None
   * @brief     Empty log data
   */
  struct None {};

  /**
   * @struct    LogData
   * @brief     List of data to log
   * @tparam    List  Data
   */
  template<typename List>
  struct LogData {
    /** Data to log */
    List list;
  };

  /**
   * @name  operator<<
   * @brief Concatenation operator
   * @param[in] first   First item, previous object
   * @param[in] value   Value of the list item
   * @return    LogData object with pair inside (chained list)
   */
  template<typename First, typename Value>
  friend constexpr LogData<std::pair<First&&, Value&&>> operator<<(LogData<First>&& first,
                                                            Value&& value) noexcept {
    return {{std::forward<First>(first.list), std::forward<Value>(value)}};
  }

  /**
   * @name  operator<<
   * @brief Concatenation operator
   * @param[in] first   First item, previous object
   * @param[in] value   Value of the list item
   * @return    LogData object with pair inside (chained list)
   */
  template<typename First, size_t n>
  friend constexpr LogData<std::pair<First&&, const char*>> operator<<(LogData<First>&& first,
                                                                       const char (&value)[n]) noexcept {
    return {{std::forward<First>(first.list), value}};
  }

  /**
   * @name  operator<<
   * @brief Concatenation operator
   * @param[in] first   First item, previous object
   * @param[in] value   Value of the list item
   * @return    LogData object with pair inside (chained list)
   */
  template<typename First>
  friend constexpr LogData<std::pair<First&&, std::ostream&>> operator<<(LogData<First>&& first,
                                                                         std::ostream& value) noexcept {
    return {{std::forward<First>(first.list), value}};
  }

#pragma mark -
#pragma mark Initialization

  /**
   * @name  Instance
   * @fn    static Logger* Instance(std::ostream& stream)
   * @brief     Logger single instance
   * @param[in] stream  Where to log
   * @return    Logger instance
   */
  static Logger& Instance(std::ostream& stream = std::cout);

#pragma mark -
#pragma mark Usage

  /**
   * @name  Log
   * @fn    static void Log(const Level level,
                            const char* file,
                            const int line,
                            const LogData<List>&& data)
   * @brief Method to invoke for logging data in a given stream.
   * @param[in] level   Level of logging
   * @param[in] file    File from where the log has been emitted
   * @param[in] line    Line number where log has been emitted
   * @param[in] data    Message to log
   */
  template<typename List>
  void Log(const Level level,
           const char* file,
           const int line,
           LogData<List>&& data)   {
    // Header, only if enable and with proper level
    if (enable_ && level <= log_level_) {
      stream_ << file << ":" << line << ": ";
      stream_ << Logger::LvlToStr[level] << " : ";
      stream_ << std::string(level > kDebug ? level - kDebug : 0, '\t');
      // Message
      Write(std::move(data.list));
      // Close
      stream_ << std::endl;
    }
  }

  /**
   * @name  Write
   * @fn    void Write(std::pair<First, Last>&& data)
   * @brief Dump data into a given stream
   * @param[in] data    Data to output
   */
  template<typename First,  typename Last>
  void Write(std::pair<First, Last>&& data) {
    Write(std::move(data.first));
    stream_ << data.second;
  }

  /**
   * @name  Write
   * @fn    void Write(None)
   * @brief Empty dump
   */
  void Write(None);

  /**
   * @name  Enable
   * @fn    void Enable(void)
   * @brief Enable logging capability
   */
  void Enable(void);

  /**
   * @name  Disable
   * @fn    void Disable(void)
   * @brief Disable logging capability
   */
  void Disable(void);

#pragma mark -
#pragma mark Accessors

  /**
   * @name  set_log_level
   * @fn    void set_log_level(const Logger::Level level)
   * @brief Define the level of log to record
   * @param[in] level   Desired level of logging
   */
  void set_log_level(const Logger::Level level) {
    log_level_ = level;
  }

  /**
   * @name  get_log_level
   * @fn    const Logger::Level& get_log_level(void) const
   * @brief Provide current level of logging
   * @return    Logging level
   */
  const Logger::Level& get_log_level(void) const {
    return log_level_;
  }

  /**
   * @name  get_log_level
   * @fn    Logger::Level& get_log_level(void)
   * @brief Provide current level of logging
   * @return    Logging level
   */
  Logger::Level& get_log_level(void) {
    return log_level_;
  }


#pragma mark -
#pragma mark Private

 private:

  /**
   * @name  Logger
   * @fn    explicit Logger(void)
   * @brief Constructor
   * @param[in] stream  Where output logging message
   */
  explicit Logger(std::ostream& stream) : stream_(stream),
                                          enable_(true),
                                          log_level_(kDebug) {}

  /** Where data will be output */
  std::ostream& stream_;
  /** Enable/Disable flag */
  bool enable_;
  /** Logging level */
  Level log_level_;
};
  
}  // namespace FaceKit
#endif //__FACEKIT_LOGGER__
