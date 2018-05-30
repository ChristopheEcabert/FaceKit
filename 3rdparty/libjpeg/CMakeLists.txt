# External project
# See : https://gist.github.com/oneamtu/3734295

# Enable CMake External project
INCLUDE(ExternalProject)

# set the build type
IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Release CACHE STRING
      "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel"
      FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

# Add libjpeg-turbo
ExternalProject_Add(libjpeg_project
        GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
        # TIMEOUT 10
        # # Force separate output paths for debug and release builds to allow easy
        # # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
        # CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
        #            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
        #            -Dgtest_force_shared_crt=ON
        CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DENABLE_SHARED=OFF
        CMAKE_CACHE_ARGS -DCMAKE_C_FLAGS:STRING=-fPIC
        # Folder destination
        PREFIX ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libjpeg
        # Disable install step
        INSTALL_COMMAND cmake -E echo "Skipping install step."
        # Wrap download, configure and build steps in a script to log output
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON)
        
# Library
ExternalProject_Get_Property(libjpeg_project binary_dir)
SET(LIBJPEG_LIBRARY_PATH ${binary_dir}/${CMAKE_STATIC_LIBRARY_PREFIX}jpeg.a)
ADD_LIBRARY(libjpeg STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET libjpeg PROPERTY IMPORTED_LOCATION ${LIBJPEG_LIBRARY_PATH})
ADD_DEPENDENCIES(libjpeg libjpeg_project)
# Export libraries
SET(JPEG_LIBRARIES libjpeg PARENT_SCOPE)
# Specify include dir
ExternalProject_Get_Property(libjpeg_project source_dir)
SET(JPEG_INCLUDE_DIRS ${source_dir} ${binary_dir} PARENT_SCOPE)