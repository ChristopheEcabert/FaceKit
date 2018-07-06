# External project
# See : https://gist.github.com/oneamtu/3734295

# Enable CMake External project
INCLUDE(ExternalProject)

# set the build type if not already set
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel"
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Add googletest
ExternalProject_Add(gtest_project
        GIT_REPOSITORY https://github.com/google/googletest
        # TIMEOUT 10
        # # Force separate output paths for debug and release builds to allow easy
        # # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
        # CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
        #            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
        CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS} -DINSTALL_GTEST=OFF -DINSTALL_GMOCK=OFF
        CMAKE_CACHE_ARGS -DCMAKE_C_FLAGS:STRING=-fPIC
        # Folder destination
        PREFIX ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/google
        # Disable install step
        INSTALL_COMMAND cmake -E echo "Skipping install step."
        # Wrap download, configure and build steps in a script to log output
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON)

# Library
ExternalProject_Get_Property(gtest_project binary_dir)
ExternalProject_Get_Property(gtest_project source_dir)
# GTest
SET(GTEST_LIBRARY_PATH ${binary_dir}/googlemock/gtest/${CMAKE_STATIC_LIBRARY_PREFIX}gtest.a)
ADD_LIBRARY(google::gtest STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET google::gtest PROPERTY IMPORTED_LOCATION ${GTEST_LIBRARY_PATH} )
SET_PROPERTY(TARGET google::gtest PROPERTY INCLUDE_DIRECTORIES "${source_dir}/googletest/include")
ADD_DEPENDENCIES(google::gtest gtest_project)
# GMock
SET(GMOCK_LIBRARY_PATH ${binary_dir}/googlemock/${CMAKE_STATIC_LIBRARY_PREFIX}gmock.a)
ADD_LIBRARY(google::gmock STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET google::gmock PROPERTY IMPORTED_LOCATION ${GMOCK_LIBRARY_PATH} )
SET_PROPERTY(TARGET google::gmock PROPERTY INCLUDE_DIRECTORIES "${source_dir}/googlemock/include")
ADD_DEPENDENCIES(google::gmock gtest_project)
