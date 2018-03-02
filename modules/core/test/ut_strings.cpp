/**
 *  @file   ut_strings.cpp
 *  @brief Unit test for string
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   2.02.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <limits>

#include "gtest/gtest.h"

#include "facekit/core/utils/string.hpp"

TEST(Path, Join) {
  namespace FK = FaceKit;
  
  EXPECT_EQ(FK::Path::Join("", "dir", "file"), "dir/file");
  EXPECT_EQ(FK::Path::Join("dir", "", "file"), "dir/file");
  EXPECT_EQ(FK::Path::Join("", "/dir", "file"), "/dir/file");
  EXPECT_EQ(FK::Path::Join("", "dir", "file/"), "dir/file/");
  
  EXPECT_EQ(FK::Path::Join("dir", "subdir", "file"), "dir/subdir/file");
  EXPECT_EQ(FK::Path::Join("dir", "subdir/", "file/"), "dir/subdir/file/");
  EXPECT_EQ(FK::Path::Join("/dir/", "/subdir", "file"), "/dir/subdir/file");
}

TEST(Path, IsAbs) {
  namespace FK = FaceKit;
  
  EXPECT_FALSE(FK::Path::IsAbsolute(""));
  EXPECT_FALSE(FK::Path::IsAbsolute("dir"));
  EXPECT_FALSE(FK::Path::IsAbsolute("./dir"));
  EXPECT_FALSE(FK::Path::IsAbsolute("../dir"));
  EXPECT_TRUE(FK::Path::IsAbsolute("/dir"));
  EXPECT_TRUE(FK::Path::IsAbsolute("/dir/../dir2"));
}

TEST(Path, Dirname) {
  namespace FK = FaceKit;
  
  EXPECT_EQ(FK::Path::Dirname(""), "");
  EXPECT_EQ(FK::Path::Dirname("Hello"), "");
  EXPECT_EQ(FK::Path::Dirname("Hello/World"), "Hello");
  EXPECT_EQ(FK::Path::Dirname("../Hello"), "..");
  EXPECT_EQ(FK::Path::Dirname("/Hello"), "/");
}

TEST(Path, Basename) {
  namespace FK = FaceKit;
  
  EXPECT_EQ(FK::Path::Basename(""), "");
  EXPECT_EQ(FK::Path::Basename("/"), "");
  EXPECT_EQ(FK::Path::Basename("foo/"), "");
  EXPECT_EQ(FK::Path::Basename("foo"), "foo");
  EXPECT_EQ(FK::Path::Basename("/foo/"), "");
  EXPECT_EQ(FK::Path::Basename("foo.txt"), "foo.txt");
  EXPECT_EQ(FK::Path::Basename("foo/bar"), "bar");
  
}

TEST(Path, Extension) {
  namespace FK = FaceKit;
  
  EXPECT_EQ(FK::Path::Extension("foo.txt"), "txt");
  EXPECT_EQ(FK::Path::Extension("foo."), "");
  EXPECT_EQ(FK::Path::Extension("foo/bar.txt"), "txt");
  EXPECT_EQ(FK::Path::Extension("foo"), "");
}


TEST(Path, SplitComponet) {
  namespace FK = FaceKit;
  
  { // Dir only
    std::string dir;
    FK::Path::SplitComponent("foo/bar.txt", &dir, nullptr, nullptr);
    EXPECT_EQ(dir, "foo");
    FK::Path::SplitComponent("bar.txt", &dir, nullptr, nullptr);
    EXPECT_EQ(dir, "");
  }
  { // File only
    std::string file;
    FK::Path::SplitComponent("foo/bar.txt", nullptr, &file, nullptr);
    EXPECT_EQ(file, "bar");
    FK::Path::SplitComponent("foo.", nullptr, &file, nullptr);
    EXPECT_EQ(file, "foo");
  }
  { // Ext only
    std::string ext;
    FK::Path::SplitComponent("foo/bar.txt", nullptr, nullptr, &ext);
    EXPECT_EQ(ext, "txt");
    FK::Path::SplitComponent("foo.", nullptr, nullptr, &ext);
    EXPECT_EQ(ext, "");
    FK::Path::SplitComponent("foo", nullptr, nullptr, &ext);
    EXPECT_EQ(ext, "");
  }
  { // All
    std::string dir, file, ext;
    FK::Path::SplitComponent("foo/bar.txt", &dir, &file, &ext);
    EXPECT_EQ(dir, "foo");
    EXPECT_EQ(file, "bar");
    EXPECT_EQ(ext, "txt");
    FK::Path::SplitComponent("baz", &dir, &file, &ext);
    EXPECT_EQ(dir, "");
    EXPECT_EQ(file, "baz");
    EXPECT_EQ(ext, "");
    FK::Path::SplitComponent("foo.bar", &dir, &file, &ext);
    EXPECT_EQ(dir, "");
    EXPECT_EQ(file, "foo");
    EXPECT_EQ(ext, "bar");
    FK::Path::SplitComponent("foo/bar", &dir, &file, &ext);
    EXPECT_EQ(dir, "foo");
    EXPECT_EQ(file, "bar");
    EXPECT_EQ(ext, "");
  }
}

TEST(Path, Clean) {
  namespace FK = FaceKit;

  EXPECT_EQ(FK::Path::Clean(""), ".");
  EXPECT_EQ(FK::Path::Clean("foo"), "foo");
  EXPECT_EQ(FK::Path::Clean("/a/b/c"), "/a/b/c");
  EXPECT_EQ(FK::Path::Clean("/a/b/c/"), "/a/b/c");
  EXPECT_EQ(FK::Path::Clean("/a//b/c/"), "/a/b/c");
  EXPECT_EQ(FK::Path::Clean("//a//b//c/"), "/a/b/c");
  EXPECT_EQ(FK::Path::Clean("/././"), "/");
  EXPECT_EQ(FK::Path::Clean("./././"), ".");
  EXPECT_EQ(FK::Path::Clean("././a/"), "a");
  EXPECT_EQ(FK::Path::Clean("a/b/.."), "a");
  EXPECT_EQ(FK::Path::Clean("a//b//c//.."), "a/b");
  EXPECT_EQ(FK::Path::Clean("../../a/b"), "../../a/b");
  EXPECT_EQ(FK::Path::Clean("a/../../.."), "../..");
}


int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
