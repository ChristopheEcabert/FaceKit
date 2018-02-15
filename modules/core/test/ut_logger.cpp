/**
 *  @file   test_logger.cpp
 *  @brief  Logger test unit
 *
 *  @author Christophe Ecabert
 *  @date   22/11/2016
 *  Copyright (c) 2016 Christophe Ecabert. All rights reserved.
 */

#include <sstream>

#include "gtest/gtest.h"

#include "facekit/core/logger.hpp"
#include "facekit/core/string_util.hpp"

#ifndef WIN32
#define ERROR_STR " \033[31mERROR\033[0m "
#define WARNING_STR " \033[33mWARNING\033[0m "
#define INFO_STR " \033[32mINFO\033[0m "
#define DEBUG_STR " \033[37mDEBUG\033[0m "
#define DEBUG1_STR " \033[37mDEBUG1\033[0m "
#define DEBUG2_STR " \033[37mDEBUG2\033[0m "
#else
#define ERROR_STR " ERROR "
#define WARNING_STR " WARNING "
#define INFO_STR " INFO "
#define DEBUG_STR " DEBUG "
#define DEBUG1_STR " DEBUG1 "
#define DEBUG2_STR " DEBUG2 "
#endif

static std::ostringstream stream;

TEST(Logger, LoggerDebug) {
  stream.str("");
  // Log
  FACEKIT_LOG_DEBUG("This is a debug entry");
  // Check for output
  std::vector<std::string> log_part;
  FaceKit::String::Split(stream.str(), ":", &log_part);
  // Check line number
  EXPECT_EQ(std::atoi(log_part[1].c_str()), __LINE__ - 5);
  // Check level indicator
  EXPECT_EQ(log_part[2], DEBUG_STR);
  // Check message
  EXPECT_EQ(log_part[3], " This is a debug entry\n");
}

TEST(Logger, LoggerInfo) {
  stream.str("");
  // Log
  FACEKIT_LOG_INFO("This is an info entry");
  // Check for output
  std::vector<std::string> log_part;
  FaceKit::String::Split(stream.str(), ":", &log_part);
  // Check line number
  EXPECT_EQ(std::atoi(log_part[1].c_str()),  __LINE__ - 5);
  // Check level indicator
  EXPECT_EQ(log_part[2], INFO_STR);
  // Check message
  EXPECT_EQ(log_part[3], " This is an info entry\n");
}

TEST(Logger, LoggerWarning) {
  stream.str("");
  // Log
  FACEKIT_LOG_WARNING("This is a warning entry");
  // Check for output
  std::vector<std::string> log_part;
  FaceKit::String::Split(stream.str(), ":", &log_part);
  // Check line number
  EXPECT_EQ(std::atoi(log_part[1].c_str()),  __LINE__ - 5);
  // Check level indicator
  EXPECT_EQ(log_part[2], WARNING_STR);
  // Check message
  EXPECT_EQ(log_part[3], " This is a warning entry\n");
}

TEST(Logger, LoggerError) {
  stream.str("");
  // Log
  FACEKIT_LOG_ERROR("This is an error entry with value " << 42);
  // Check for output
  std::vector<std::string> log_part;
  FaceKit::String::Split(stream.str(), ":", &log_part);
  // Check line number
  EXPECT_EQ(std::atoi(log_part[1].c_str()),  __LINE__ - 5);
  // Check level indicator
  EXPECT_EQ(log_part[2], ERROR_STR);
  // Check message
  EXPECT_EQ(log_part[3], " This is an error entry with value 42\n");
}

TEST(Logger, LoggerDebugLvl1) {
  stream.str("");
  FaceKit::Logger::Instance().set_log_level(FaceKit::Logger::Level::kDebug2);
  // Log
  FACEKIT_LOG_DEBUG1("This is a level 1 debug entry");
  // Check for output
  std::vector<std::string> log_part;
  FaceKit::String::Split(stream.str(), ":", &log_part);
  // Check line number
  EXPECT_EQ(std::atoi(log_part[1].c_str()),  __LINE__ - 5);
  // Check level indicator
  EXPECT_EQ(log_part[2], DEBUG1_STR);
  // Check message
  EXPECT_EQ(log_part[3], " \tThis is a level 1 debug entry\n");
}

TEST(Logger, LoggerDebugLvl2) {
  stream.str("");
  FaceKit::Logger::Instance().set_log_level(FaceKit::Logger::Level::kDebug2);
  // Log
  FACEKIT_LOG_DEBUG2("This is a level 2 debug entry");
  // Check for output
  std::vector<std::string> log_part;
  FaceKit::String::Split(stream.str(), ":", &log_part);
  // Check line number
  EXPECT_EQ(std::atoi(log_part[1].c_str()),  __LINE__ - 5);
  // Check level indicator
  EXPECT_EQ(log_part[2], DEBUG2_STR);
  // Check message
  EXPECT_EQ(log_part[3], " \t\tThis is a level 2 debug entry\n");
}

TEST(Logger, LoggerDisable) {
  stream.str("");
  FaceKit::Logger::Instance().Disable();
  // Log
  FACEKIT_LOG_INFO("This should not be logged");
  // Check for output
  EXPECT_TRUE(stream.str().empty());
  // Enable back
  FaceKit::Logger::Instance().Enable();
}

TEST(Logger, LoggerFilter) {
  stream.str("");
  FaceKit::Logger::Instance().set_log_level(FaceKit::Logger::Level::kDebug);
  // Log
  FACEKIT_LOG_DEBUG1("This should not be logged");
  // Check for output
  EXPECT_TRUE(stream.str().empty());
  // Log
  FaceKit::Logger::Instance().set_log_level(FaceKit::Logger::Level::kDebug1);
  FACEKIT_LOG_DEBUG1("This is level 1");
  FACEKIT_LOG_DEBUG2("This should not be logged");
  // Check for output
  std::vector<std::string> log_part;
  FaceKit::String::Split(stream.str(), ":", &log_part);
  // Check line number
  EXPECT_EQ(std::atoi(log_part[1].c_str()),  __LINE__ - 6);
  // Check level indicator
  EXPECT_EQ(log_part[2], DEBUG1_STR);
  // Check message
  EXPECT_EQ(log_part[3], " \tThis is level 1\n");
}

int main(int argc, char* argv[]) {
  // Create stringstream + Init log
  FaceKit::Logger::Instance(stream);  
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
