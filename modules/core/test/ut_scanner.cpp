/**
 *  @file   ut_scanner.cpp
 *  @brief Scanner Unit Test
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   25.05.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include "gtest/gtest.h"

#include "facekit/core/utils/scanner.hpp"
namespace FK = FaceKit::String;
using CharType = FK::Scanner::CharType;
using Scanner = FK::Scanner;

/**
 *  @namespace  FaceKit
 *  @brief      Development space
 */
namespace FaceKit {
  
/**
*  @namespace  String
*  @brief      Utility functions for processing string
*/
namespace String {

class ScannerTest : public ::testing::Test {
protected:
  
  std::string CharTypeStr(const CharType& type) {
    std::string s;
    // Generate all possible ascii number
    for (int i = 0; i < 256; ++i) {
      char c = static_cast<char>(i);
      if (Scanner::Matches(type, c)) {
        s += c;
      }
    }
    return s;
  }
};
  
TEST(Scanner, Any) {
  std::string match, remain;
  EXPECT_TRUE(Scanner("HelloWorld1234")
              .Any(CharType::kLetter)
              .Any(CharType::kSpace)
              .Result(&remain, &match));
  EXPECT_EQ("1234", remain);
  EXPECT_EQ("HelloWorld", match);
  
  // Empty string
  EXPECT_TRUE(Scanner("")
              .Any(CharType::kLetter)
              .Any(CharType::kNumber)
              .Result(&remain, &match));  // Zero match consider as correct
  EXPECT_EQ("", remain);
  EXPECT_EQ("", match);
  
  // Symbol
  EXPECT_TRUE(Scanner("*+.....+*")
              .Any(CharType::kLetter)
              .Any(CharType::kNumber)
              .Result(&remain, &match));
  EXPECT_EQ("*+.....+*", remain);
  EXPECT_EQ("", match);
}

TEST(Scanner, AnySpace) {
  std::string match, remain;
  EXPECT_TRUE(Scanner("  1\t 2 ")
              .AnySpace()
              .One(CharType::kNumber)
              .AnySpace()
              .Result(&remain, &match));
  EXPECT_EQ("2 ", remain);
  EXPECT_EQ("  1\t ", match);
}

TEST(Scanner, AnyEscapedNewLine) {
  std::string match, remain;
  EXPECT_TRUE(Scanner("\\\n")
              .Any(CharType::kLetterNumberUnderscore)
              .Result(&remain, &match));
  EXPECT_EQ("\\\n", remain);
  EXPECT_EQ("", match);
}

TEST(Scanner, AnyEmptys) {
  std::string match, remain;
  EXPECT_TRUE(Scanner("")
              .Any(CharType::kLetterNumberUnderscore)
              .Result(&remain, &match));
  EXPECT_EQ("", remain);
  EXPECT_EQ("", match);
}

TEST(Scanner, One) {
  // Check with valid, invalid, empty
  EXPECT_TRUE(Scanner("HiThere")
              .One(CharType::kLetter)
              .Result());
  EXPECT_FALSE(Scanner("1234")
               .One(CharType::kLetter)
               .Result());
  EXPECT_FALSE(Scanner("")
               .One(CharType::kLetter)
               .Result());
  
  // check result
  std::string match, remain;
  EXPECT_TRUE(Scanner("HiThere")
              .One(CharType::kUpperLetter)
              .One(CharType::kLowerLetter)
              .Result(&remain, &match));
  EXPECT_EQ("Hi", match);
  EXPECT_EQ("There", remain);
  
  EXPECT_TRUE(Scanner("1")
              .One(CharType::kNumber)
              .Result(&remain, &match));
  EXPECT_EQ("1", match);
  EXPECT_EQ("", remain);
}

TEST(Scanner, OneLiteral) {
  EXPECT_FALSE(Scanner("hello")
               .OneLiteral("Hello")
               .Result());
  EXPECT_TRUE(Scanner("Hello")
              .OneLiteral("Hell")
              .OneLiteral("o")
              .Result());
}

TEST(Scanner, ZeroOrOneLiteral) {
  std::string remain, match;
  EXPECT_TRUE(Scanner("helloWorld")
              .ZeroOrOneLiteral("Hello")
              .Result(&remain, &match));
  EXPECT_EQ("helloWorld", remain);
  EXPECT_EQ("", match);
  
  EXPECT_TRUE(Scanner("HelloWorld")
              .ZeroOrOneLiteral("Hell")
              .ZeroOrOneLiteral("o")
              .Result(&remain, &match));
  EXPECT_EQ("World", remain);
  EXPECT_EQ("Hello", match);
  
  EXPECT_TRUE(Scanner("")
              .ZeroOrOneLiteral("hello")
              .Result(&remain, &match));
  EXPECT_EQ("", remain);
  EXPECT_EQ("", match);
}

TEST(Scanner, Many) {
  std::string remain, match;
  EXPECT_TRUE(Scanner("HelloWorld")
              .Many(CharType::kLetter)
              .Result());
  EXPECT_FALSE(Scanner("0123")
               .Many(CharType::kLetter)
               .Result());
  EXPECT_FALSE(Scanner("")
               .Many(CharType::kLetter)
               .Result());
  
  EXPECT_TRUE(Scanner("HiThere 1234")
              .Many(CharType::kLetter)
              .Result(&remain, &match));
  EXPECT_EQ(" 1234", remain);
  EXPECT_EQ("HiThere", match);
  
  EXPECT_TRUE(Scanner("HelloWorld")
              .Many(CharType::kLetter)
              .Result(&remain, &match));
  EXPECT_EQ("", remain);
  EXPECT_EQ("HelloWorld", match);
}

TEST(Scanner, EndOfString) {
  EXPECT_FALSE(Scanner("a")
               .Eos()
               .Result());
  EXPECT_TRUE(Scanner("")
              .Eos()
              .Result());
  EXPECT_FALSE(Scanner("abc")
               .OneLiteral("ab")
               .Eos()
               .Result());
  EXPECT_TRUE(Scanner("abc")
              .OneLiteral("abc")
              .Eos()
              .Result());
}

TEST(Scanner, ScanUntil) {
  std::string remain, match;
  EXPECT_TRUE(Scanner(R"(' \a \b \c \' \\'part)")
              .OneLiteral("'")
              .ScanUntil('\'')
              .OneLiteral("'")
              .Result(&remain, &match));
  EXPECT_EQ(R"( \\'part)", remain);
  EXPECT_EQ(R"(' \a \b \c \')", match);
  
  // The "scan until" character is not present.
  remain = match = "unset";
  EXPECT_FALSE(Scanner(R"(' \a \b \c \\part)")
               .OneLiteral("'")
               .ScanUntil('\'')
               .Result(&remain, &match));
  EXPECT_EQ("unset", remain);
  EXPECT_EQ("unset", match);
  
  // Scan until an escape character.
  remain = match = "";
  EXPECT_TRUE(Scanner(R"(abc\def)")
              .ScanUntil('\\')
              .Result(&remain, &match));
  EXPECT_EQ(R"(\def)", remain);
  EXPECT_EQ("abc", match);
}

TEST(Scanner, ScanUntilEscaped) {
  std::string remain, match;
  EXPECT_TRUE(Scanner(R"(' \a \b \c \' \\'part)")
              .OneLiteral("'")
              .ScanEscapedUntil('\'')
              .OneLiteral("'")
              .Result(&remain, &match));
  EXPECT_EQ("part", remain);
  EXPECT_EQ(R"(' \a \b \c \' \\')", match);
  
  // The "scan until" character is not present.
  remain = match = "unset";
  EXPECT_FALSE(Scanner(R"(' \a \b \c \' \\rest)")
               .OneLiteral("'")
               .ScanEscapedUntil('\'')
               .Result(&remain, &match));
  EXPECT_EQ("unset", remain);
  EXPECT_EQ("unset", match);
}

TEST(Scanner, CaptureAndResult) {
  std::string remain, match;
  
  Scanner scan("  hello    world");
  EXPECT_TRUE(scan
              .Any(CharType::kSpace)
              .RestartCapture()
              .One(CharType::kLetter)
              .Any(CharType::kLetterNumber)
              .StopCapture()
              .Any(CharType::kSpace)
              .Result(&remain, &match));
  EXPECT_EQ("world", remain);
  EXPECT_EQ("hello", match);
  EXPECT_TRUE(scan.Result());
  
  remain = "";
  EXPECT_TRUE(scan.Result(&remain));
  EXPECT_EQ("world", remain);
  
  remain = "";
  match = "";
  EXPECT_TRUE(scan.Result(&remain, &match));
  EXPECT_EQ("world", remain);
  EXPECT_EQ("hello", match);
  
  scan.RestartCapture().One(CharType::kLetter).One(CharType::kLetter);
  remain = "";
  match = "";
  EXPECT_TRUE(scan.Result(&remain, &match));
  EXPECT_EQ("rld", remain);
  EXPECT_EQ("wo", match);
}

TEST(Scanner, ExtendedCaptureAndResult) {
  std::string remain, match;
  Scanner scan("Hello2Everyone");
  EXPECT_TRUE(scan
              .Many(CharType::kLetter)
              .Result(&remain, &match));
  EXPECT_EQ("2Everyone", remain);
  EXPECT_EQ("Hello", match);
  EXPECT_TRUE(scan.Many(CharType::kNumber).Result(&remain, &match));
  EXPECT_EQ("Everyone", remain);
  EXPECT_EQ("Hello2", match);
  EXPECT_TRUE(scan.Many(CharType::kLetter).Result(&remain, &match));
  EXPECT_EQ("", remain);
  EXPECT_EQ("Hello2Everyone", match);
}
  
TEST(Scanner, FailedMAtch) {
  // A failed match doesn't change pointers passed to GetResult.
  std::string remain = "rem";
  std::string match = "match";
  Scanner scan("name");
  EXPECT_FALSE(scan.One(CharType::kSpace).Result(&remain, &match));
  EXPECT_EQ("rem", remain);
  EXPECT_EQ("match", match);
}

TEST_F(ScannerTest, AllCharType) {
  // Check all ascii char are generated
  EXPECT_EQ(256, CharTypeStr(CharType::kAll).size());
  
  // Numbers
  EXPECT_EQ("0123456789", CharTypeStr(CharType::kNumber));
  // Non zero numbers
  EXPECT_EQ("123456789", CharTypeStr(CharType::kNonZeroNumber));
  // Lower letters
  EXPECT_EQ("abcdefghijklmnopqrstuvwxyz", CharTypeStr(CharType::kLowerLetter));
  // Lower letter + number
  EXPECT_EQ("0123456789abcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLowerLetterNumber));
  // Capital letters
  EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZ", CharTypeStr(CharType::kUpperLetter));
  // Letters
  EXPECT_EQ("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetter));
  // Letters + numbers
  EXPECT_EQ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumber));
  // Spaces
  EXPECT_EQ("\t\n\v\f\r ", CharTypeStr(CharType::kSpace));
  // Lower letters + number + _
  EXPECT_EQ("0123456789_abcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLowerLetterNumberUnderscore));
  // Letters + numbers + _
  EXPECT_EQ("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumberUnderscore));
  // Letters + numbers + - + _
  EXPECT_EQ("-0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumberDashUnderscore));
  // Letters + numbers + .
  EXPECT_EQ(".0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumberDot));
  // Letters + numbers + . + _
  EXPECT_EQ(".0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumberDotUnderscore));
  // Letters + numbers + . +/-
  EXPECT_EQ("+-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumberPlusMinusDot));
  // Letter + number + - + . + /
  EXPECT_EQ("-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumberDashDotSlash));
  // Letter + number + - + . + / + _
  EXPECT_EQ("-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz",
            CharTypeStr(CharType::kLetterNumberDashDotSlashUnderscore));
}

}  // namespace String
}  // namespace FaceKit


int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
