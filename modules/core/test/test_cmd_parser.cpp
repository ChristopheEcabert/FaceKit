/**
 *  @file   test_cmd_parser.cpp
 *  @brief  Unit test for command line parser
 *
 *  @author Christophe Ecabert
 *  @date   28/18/16
 *  Copyright (c) 2016 Christophe Ecabert. All rights reserved.
 */

#include <stdio.h>
#include <iostream>

#include "gtest/gtest.h"

#include "facekit/core/cmd_parser.hpp"

using CmdParser = FaceKit::CmdLineParser;

TEST(Parser, NoArgs) {
  CmdParser parser;
  int argc = 1;
  const char* argv[] = {"exec_name"};
  int e = parser.ParseCmdLine(argc, argv);
  EXPECT_EQ(e, -1);
}

TEST(Parser, ArgsNeed) {
  CmdParser parser;
  parser.AddArgument("-a", CmdParser::ArgState::kNeeded, "arg 1");
  int argc = 3;
  const char* argv[] = {"exec_name", "-a", "arg_name"};
  int e = parser.ParseCmdLine(argc, argv);
  // Check parsing
  EXPECT_EQ(e, 0);
  if (!e) {
    // Check query
    std::string value;
    parser.HasArgument("-a", &value);
    EXPECT_EQ(value, "arg_name");
  }
}

TEST(Parser, ArgsNeedMissing) {
  CmdParser parser;
  parser.AddArgument("-a", CmdParser::ArgState::kNeeded, "arg 1");
  int argc = 1;
  const char* argv[] = {"exec_name"};
  int e = parser.ParseCmdLine(argc, argv);
  // Check parsing
  EXPECT_EQ(e, -1);
}

TEST(Parser, ArgsOptional) {
  CmdParser parser;
  parser.AddArgument("-a", CmdParser::ArgState::kOptional, "arg 1");
  int argc = 1;
  const char* argv[] = {"exec_name"};
  int e = parser.ParseCmdLine(argc, argv);
  // Check parsing
  EXPECT_EQ(e, 0);
  if (!e) {
    // Check query
    std::string value;
    parser.HasArgument("-a", &value);
    EXPECT_EQ(value, "");
  }
}

TEST(Parser, ArgsOptionalPresent) {
  CmdParser parser;
  parser.AddArgument("-a", CmdParser::ArgState::kOptional, "arg 1");
  int argc = 3;
  const char* argv[] = {"exec_name","-a","value1"};
  int e = parser.ParseCmdLine(argc, argv);
  // Check parsing
  EXPECT_EQ(e, 0);
  if (!e) {
    // Check query
    std::string value;
    parser.HasArgument("-a", &value);
    EXPECT_EQ(value, "value1");
  }
}

TEST(Parser, ArgsNeedAndOptional) {
  CmdParser parser;
  parser.AddArgument("-a", CmdParser::ArgState::kNeeded, "arg 1");
  parser.AddArgument("-b", CmdParser::ArgState::kOptional, "arg 2");
  int argc = 3;
  const char* argv[] = {"exec_name","-a", "value1"};
  int e = parser.ParseCmdLine(argc, argv);
  // Check parsing
  EXPECT_EQ(e, 0);
  if (!e) {
    // Check query
    std::string value1;
    parser.HasArgument("-a", &value1);
    EXPECT_EQ(value1, "value1");
    std::string value2;
    parser.HasArgument("-b", &value2);
    EXPECT_EQ(value2, "");
  }
}


TEST(Parser, ArgsNeedAndOptionalPresent) {
  CmdParser parser;
  parser.AddArgument("-a", CmdParser::ArgState::kNeeded, "arg 1");
  parser.AddArgument("-b", CmdParser::ArgState::kOptional, "arg 2");
  int argc = 5;
  const char* argv[] = {"exec_name","-a", "value1","-b", "value2"};
  int e = parser.ParseCmdLine(argc, argv);
  // Check parsing
  EXPECT_EQ(e, 0);
  if (!e) {
    // Check query
    std::string value1;
    parser.HasArgument("-a", &value1);
    EXPECT_EQ(value1, "value1");
    std::string value2;
    parser.HasArgument("-b", &value2);
    EXPECT_EQ(value2, "value2");
  }
}

int main(int argc, const char * argv[]) {
  ::testing::InitGoogleTest(&argc, const_cast<char**>(argv));
  return RUN_ALL_TESTS();
}
