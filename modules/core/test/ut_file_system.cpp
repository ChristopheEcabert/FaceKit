/**
 *  @file   ut_file_system.cpp
 *  @brief Unit test for posix file system
 *  @ingroup core
 *
 *  @author Christophe Ecabert
 *  @date   05.03.18
 *  Copyright © 2018 Christophe Ecabert. All rights reserved.
 */

#include <limits>

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "facekit/core/sys/posix_file_system.hpp"
#include "facekit/core/sys/file_system_factory.hpp"


class FileSystemTest : public ::testing::Test {
 public:
  
  FileSystemTest(void) : fs_(new FaceKit::PosixFileSystem()) {}
  
  ~FileSystemTest(void) {
    if (fs_) {
      delete fs_;
    }
  }
  
 
 protected:
  void SetUp(void) {
    // Create folders
    std::system("mkdir -p ut_filesystem");
    
    // Create test structure
    // <root>/hello_world.txt
    // <root>/foo_bar.txt
    // <root>/subdir
    // <root>/subdir/item1.txt
    // <root>/subdir/item2.txt
    // <root>/to_rm
    // <root>/to_rm/remove_me.txt
    // <root>/to_rm/subdir
    // <root>/to_rm/subdir/remove_me_too.txt
    // <root>/empty
    
    
    std::system("echo HelloWorld! >> ut_filesystem/hello_world.txt");
    std::system("echo FooBar! >> ut_filesystem/foo_bar.txt");
    std::system("mkdir -p ut_filesystem/subdir");
    std::system("echo Item1! >> ut_filesystem/subdir/item1.txt");
    std::system("echo Item2! >> ut_filesystem/subdir/item2.txt");
    std::system("mkdir -p ut_filesystem/to_rm");
    std::system("echo RM Me! >> ut_filesystem/to_rm/remove_me.txt");
    std::system("mkdir -p ut_filesystem/to_rm/subdir");
    std::system("echo RM Me too! >> ut_filesystem/to_rm/subdir/remove_me_too.txt");
    std::system("mkdir -p ut_filesystem/empty");
  }
  
  
  void TearDown(void) {
    // Clean up
    std::system("rm -rf ut_filesystem");
  }
  
 public:
  /** File System */
  FaceKit::FileSystem* fs_;
};


TEST_F(FileSystemTest, FileExist) {
  namespace FK = FaceKit;
  
  // Check if file exist
  FK::Status s = this->fs_->FileExist("ut_filesystem//hello_world.txt");
  EXPECT_TRUE(s.Good());
  s = this->fs_->FileExist("ut_filesystem/foo_bar.txt");
  EXPECT_TRUE(s.Good());
  s = this->fs_->FileExist("ut_filesystem//foo_baz.txt");
  EXPECT_FALSE(s.Good());
  EXPECT_EQ(s.Message(), "ut_filesystem/foo_baz.txt not found");
  s.Clear();
  s = this->fs_->FileExist("ut_filesystem/subdir");
  EXPECT_TRUE(s.Good());
  s = this->fs_->FileExist("ut_filesystem/baz");
  EXPECT_FALSE(s.Good());
  EXPECT_EQ(s.Message(), "ut_filesystem/baz not found");
}

TEST_F(FileSystemTest, ListDir) {
  namespace FK = FaceKit;
  
  { // List roo
    std::vector<std::string> content;
    FK::Status s = this->fs_->ListDir("ut_filesystem", &content);
    std::sort(content.begin(), content.end());
    EXPECT_TRUE(s.Good());
    EXPECT_THAT(content,
                ::testing::ElementsAre("ut_filesystem/empty",
                                       "ut_filesystem/foo_bar.txt",
                                       "ut_filesystem/hello_world.txt",
                                       "ut_filesystem/subdir",
                                       "ut_filesystem/to_rm"));
  }
  { // List recursively
    std::vector<std::string> content;
    FK::Status s = this->fs_->ListDirRecursively("ut_filesystem", &content);
    std::sort(content.begin(), content.end());
    EXPECT_TRUE(s.Good());
    EXPECT_THAT(content, testing::ElementsAre("ut_filesystem/foo_bar.txt",
                                              "ut_filesystem/hello_world.txt",
                                              "ut_filesystem/subdir/item1.txt",
                                              "ut_filesystem/subdir/item2.txt",
                                              "ut_filesystem/to_rm/remove_me.txt",
                                              "ut_filesystem/to_rm/subdir/remove_me_too.txt"));
  }
  { // Check unexisting folder (recursively)
    std::vector<std::string> content;
    FK::Status s = this->fs_->ListDir("ut_filesys", &content);
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not open directory: ut_filesys");
  }
  { // Check unexisting folder (recursively)
    std::vector<std::string> content;
    FK::Status s = this->fs_->ListDirRecursively("ut_fsystem", &content);
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not open directory: ut_fsystem");
  }
}

TEST_F(FileSystemTest, FileProperties) {
  namespace FK = FaceKit;
  
  { // HelloWorld
    FK::FileProperty prop;
    FK::Status s = fs_->FileProp("ut_filesystem/hello_world.txt", &prop);
    EXPECT_TRUE(s.Good());
    EXPECT_FALSE(prop.is_dir);
    EXPECT_EQ(prop.size, 12); // 11char + CR
  }
  { // Folder
    FK::FileProperty prop;
    FK::Status s = fs_->FileProp("ut_filesystem/subdir", &prop);
    EXPECT_TRUE(s.Good());
    EXPECT_TRUE(prop.is_dir);
    EXPECT_GT(prop.size, 0); // folder
  }
  { // Unexisting file
    FK::FileProperty prop;
    FK::Status s = fs_->FileProp("ut_filesystem/hello.txt", &prop);
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not access: ut_filesystem/hello.txt");
  }
}

TEST_F(FileSystemTest, DeleteFile) {
  namespace FK = FaceKit;

  { // Delete existing file
    FK::Status s = fs_->DeleteFile("ut_filesystem/to_rm/remove_me.txt");
    EXPECT_TRUE(s.Good());
  }
  { // Delete not existing file
    FK::Status s = fs_->DeleteFile("ut_filesystem/to_rm/remove_me.txt");
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not delete file: ut_filesystem/to_rm/remove_me.txt");
  }
}

TEST_F(FileSystemTest, CreateDir) {
  namespace FK = FaceKit;
  
  { // Existing dir
    FK::Status s = fs_->CreateDir("ut_filesystem/new_dir/");
    EXPECT_TRUE(s.Good());
  }
  { // Not Existing dir
    FK::Status s = fs_->CreateDir("ut_fsystem/new_dir");
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not create: ut_fsystem/new_dir");
  }
}

TEST_F(FileSystemTest, CreateDirRecursively) {
  namespace FK = FaceKit;
  
  { // From proper dir
    FK::Status s = fs_->CreateDirRecursively("ut_filesystem/subdir1/subsubdir2");
    EXPECT_TRUE(s.Good());
    EXPECT_TRUE(fs_->FileExist("ut_filesystem/subdir1/subsubdir2").Good());
  }
}

TEST_F(FileSystemTest, IsDirectory) {
  namespace FK = FaceKit;
  
  { // Actual dir
    FK::Status s = fs_->IsDirectory("ut_filesystem/subdir/");
    EXPECT_TRUE(s.Good());
  }
  { // File
    FK::Status s = fs_->IsDirectory("ut_filesystem/subdir/item1.txt");
    EXPECT_FALSE(s.Good());
  }
  { // Inexisting
    FK::Status s = fs_->IsDirectory("ut_filesystem/subdirr/");
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Folder ut_filesystem/subdirr/ does not exists!");
  }
}

TEST_F(FileSystemTest, DeleteDir) {
  namespace FK = FaceKit;
  
  { // Try to delete not empty dir
    FK::Status s = fs_->DeleteDir("ut_filesystem/to_rm");
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not delete: ut_filesystem/to_rm");
  }
  { // Empty dir
    FK::Status s = fs_->DeleteDir("ut_filesystem/empty");
    EXPECT_TRUE(s.Good());
  }
}

TEST_F(FileSystemTest, DeleteDirRecursively) {
  namespace FK = FaceKit;
  { // Existing folder
    FK::Status s = fs_->DeleteDirRecursively("ut_filesystem/to_rm");
    EXPECT_TRUE(s.Good());
    EXPECT_FALSE(fs_->FileExist("ut_filesystem/to_rm").Good());
  }
  { // Not Existing folder
    FK::Status s = fs_->DeleteDirRecursively("ut_filesystem/to_rm_too");
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not open directory: ut_filesystem/to_rm_too");
  }
}

TEST_F(FileSystemTest, RenameFile) {
  namespace FK = FaceKit;
  { // Existing
    FK::Status s = fs_->RenameFile("ut_filesystem/subdir/item1.txt",
                                   "ut_filesystem/subdir/item3.txt");
    EXPECT_TRUE(s.Good());
    EXPECT_TRUE(fs_->FileExist("ut_filesystem/subdir/item3.txt").Good());
  }
  { // Not existing
    FK::Status s = fs_->RenameFile("ut_filesystem/subdir/item1.txt",
                                   "ut_filesystem/subdir/item3.txt");
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not rename: ut_filesystem/subdir/item1.txt");
  }
}

TEST_F(FileSystemTest, FileSize) {
  namespace FK = FaceKit;
  
  { // Existing
    size_t sz = 0;
    FK::Status s = fs_->QueryFileSize("ut_filesystem/subdir/item1.txt", &sz);
    EXPECT_TRUE(s.Good());
    EXPECT_EQ(sz, 7);
  }
  { // Not Existing
    size_t sz = 0;
    FK::Status s = fs_->QueryFileSize("ut_filesystem/subdir/item4.txt", &sz);
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(sz, 0);
  }
}

TEST_F(FileSystemTest, CopyFile) {
  namespace FK = FaceKit;
  
  { // Existing
    FK::Status s = fs_->CopyFile("ut_filesystem/subdir/item1.txt",
                                 "ut_filesystem/empty/file.txt");
    EXPECT_TRUE(s.Good());
    EXPECT_TRUE(fs_->FileExist("ut_filesystem/empty/file.txt").Good());
  }
  { // Not Existing
    FK::Status s = fs_->CopyFile("ut_filesystem/subdir/item4.txt",
                                 "ut_filesystem/empty/file.txt");
    EXPECT_FALSE(s.Good());
    EXPECT_EQ(s.Message(), "Can not copy: ut_filesystem/subdir/item4.txt");
  }
}

TEST(FileSystemFactory, Retrieve) {
  namespace FK = FaceKit;
  
  // Check it is properly registered
  FK::FileSystem* fs = FK::FileSystemFactory::Get().Retrieve("Posix");
  EXPECT_NE(fs, nullptr);
  // Check Win is registered
  fs = FK::FileSystemFactory::Get().Retrieve("Windows");
  EXPECT_NE(fs, nullptr);
  // Check it is properly registered
  fs = FK::FileSystemFactory::Get().Retrieve("NotExistingFileSystem");
  EXPECT_EQ(fs, nullptr);
}

int main(int argc, char* argv[]) {
  // Init gtest framework
  ::testing::InitGoogleTest(&argc, argv);
  // Run unit test
  return RUN_ALL_TESTS();
}
