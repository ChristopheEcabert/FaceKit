/**
 *  @file   ut_stacktrace.cpp
 *  @brief StackTrace unit-test
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   09.07.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "facekit/core/sys/stacktrace.hpp"
#include "facekit/core/sys/stacktrace_resolver.hpp"
#include "facekit/core/utils/string.hpp"
#include "facekit/core/utils/scanner.hpp"

namespace FK = FaceKit;


#ifdef DEBUG
std::string symbols[] = {"FaceKit::StackTrace::StackTrace(",
  "FaceKit::StackTrace::StackTrace(",
  "FaceKit::StackTrace::StackTrace()",
  "GenerateStackTrace"};

std::string libnames[] = {"libfacekit_core",
  "libfacekit_core",
  "libfacekit_core",
  "facekit_ut_stacktrace"};

size_t trace_range = 5;
#else
std::string symbols[] = {"FaceKit::StackTrace::StackTrace(",
  "FaceKit::StackTrace::StackTrace()",
  "GenerateStackTrace"};

std::string libnames[] = {"libfacekit_core",
  "libfacekit_core",
  "facekit_ut_stacktrace"};

size_t trace_range = 4;
#endif

/**
 *  @name   GenerateStackTrace
 *  @fn     FK::Status GenerateStackTrace(std::string* trace)
 *  @brief  Generate a stack trace
 */
FK::Status GenerateStackTrace(std::string* trace) {
  FK::StackTrace st;
  return st.ToString(trace);
}

TEST(StackTraceFrame, CTor) {
  // Predicate with default constructor
  {
    FK::StackTraceFrame frame;
    EXPECT_FALSE(frame.IsAddressKnown());
    EXPECT_FALSE(frame.IsLibraryNameKnown());
    EXPECT_FALSE(frame.IsLineNumberKnown());
    EXPECT_FALSE(frame.IsMangledSymbolNameKnown());
    EXPECT_FALSE(frame.IsSymbolNameKnown());
    EXPECT_FALSE(frame.IsOffsetKnown());
    EXPECT_FALSE(frame.IsSourceFileNameKnown());
  }
  // Predicate with specialized ctor
  {
    FK::StackTraceFrame frame((const void*)0x01,
                              "my_library.so",
                              10,
                              "MangledSymbol",
                              "Symbol",
                              0x1F,
                              "my_source_file.cpp");
    EXPECT_TRUE(frame.IsAddressKnown());
    EXPECT_TRUE(frame.IsLibraryNameKnown());
    EXPECT_TRUE(frame.IsLineNumberKnown());
    EXPECT_TRUE(frame.IsMangledSymbolNameKnown());
    EXPECT_TRUE(frame.IsSymbolNameKnown());
    EXPECT_TRUE(frame.IsOffsetKnown());
    EXPECT_TRUE(frame.IsSourceFileNameKnown());
  }
}

TEST(StackTraceFrame, GetterSetter) {
  { // Getter
    FK::StackTraceFrame frame((const void*)0x1A,
                              "my_library.so",
                              10,
                              "MangledSymbol",
                              "Symbol",
                              0x1F,
                              "my_source_file.cpp");
    
    EXPECT_EQ(frame.get_address(), (const void*)0x1A);
    EXPECT_EQ(frame.get_library_name(), "my_library.so");
    EXPECT_EQ(frame.get_line_number(), 10);
    EXPECT_EQ(frame.get_mangled_symbol_name(), "MangledSymbol");
    EXPECT_EQ(frame.get_symbol_name(), "Symbol");
    EXPECT_EQ(frame.get_offset(), 0x1F);
    EXPECT_EQ(frame.get_src_file_name(), "my_source_file.cpp");
  }
  
  { // Setter
    FK::StackTraceFrame frame;
    frame.set_address((const void*)0x2F);
    frame.set_library_name("some_lib_name.dylib");
    frame.set_line_number(42);
    frame.set_mangled_symbol_name("ZDMangledName");
    frame.set_symbol_name("Name");
    frame.set_offset(0x22);
    frame.set_src_file_name("my_file.cxx");
    
    EXPECT_EQ(frame.get_address(), (const void*)0x2F);
    EXPECT_EQ(frame.get_library_name(), "some_lib_name.dylib");
    EXPECT_EQ(frame.get_line_number(), 42);
    EXPECT_EQ(frame.get_mangled_symbol_name(), "ZDMangledName");
    EXPECT_EQ(frame.get_symbol_name(), "Name");
    EXPECT_EQ(frame.get_offset(), 0x22);
    EXPECT_EQ(frame.get_src_file_name(), "my_file.cxx");
  }
}

TEST(StackTrace, Trace) {
  using Type = FK::String::Scanner::CharType;
  using ::testing::HasSubstr;
  
  std::string trace;
  auto s = GenerateStackTrace(&trace);
  // Generation was successful ?
  EXPECT_TRUE(s.Good());
  
  // Check values
  std::vector<std::string> parts;
  FK::String::Split(trace, "\n", &parts);
  // Process each line
  for (size_t k = 1; k < trace_range; ++k) {
    // Get symbol name
    FK::String::Scanner scan(parts[k]);
    scan.OneLiteral(" * ")
    .Many(Type::kLetterNumber)
    .AnySpace()
    .RestartCapture()
    .ScanUntil(')')
    .OneLiteral(")");
    
    std::string symbol;
    scan.Result(nullptr, &symbol);
    EXPECT_THAT(symbol, HasSubstr(symbols[k-1]));
    
    // Get library name
    scan.ScanUntil('(')
    .OneLiteral("(")
    .RestartCapture()
    .ScanUntil(')');
    
    std::string lib_name;
    scan.Result(nullptr, &lib_name);
    auto pos = lib_name.find('.');
    if (pos != std::string::npos) {
      lib_name = lib_name.substr(0, pos);
    }
    EXPECT_EQ(lib_name, libnames[k-1]);
  }
}

int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
