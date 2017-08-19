/**
 *  @file   logger.cpp
 *  @brief  Logging tool
 *          Based on : http://stackoverflow.com/questions/19415845/
 *
 *  @author Christophe Ecabert
 *  @date   19/08/2017
 *  Copyright (c) 2017 Christophe Ecabert. All rights reserved.
 */

#include "facekit/core/logger.hpp"

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/** Conversion from level enumerate to string */
constexpr const char* Logger::LvlToStr[];

#pragma mark -
#pragma mark Initialization

/*
 * @name  Instance
 * @fn    static Logger* Instance(std::ostream& stream)
 * @brief     Logger single instance
 * @param[in] stream  Where to log
 * @return    Logger instance
 */
Logger& Logger::Instance(std::ostream& stream) {
  // Logger instance
  static Logger logger(stream);
  return logger;
}

#pragma mark -
#pragma mark Usage

/*
 * @name
 * @fn
 * @brief Empty dump
 * @param[in] data
 */
void Logger::Write(None) {
}

/*
 * @name  Enable
 * @fn    void Enable(void)
 * @brief Enable logging capability
 */
void Logger::Enable(void) {
  enable_ = true;
}

/*
 * @name  Disable
 * @fn    void Disable(void)
 * @brief Disable logging capability
 */
void Logger::Disable(void) {
  enable_ = false;
}
}  // namespace FaceKit
