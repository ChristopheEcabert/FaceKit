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
# Get NASM path
EXECUTE_PROCESS(COMMAND which nasm OUTPUT_VARIABLE NASM_PATH OUTPUT_STRIP_TRAILING_WHITESPACE)
# Define configure args
IF(APPLE)
  # Apple build
  IF(OS_IS_64BIT)
    SET(CONFIG_HOST "x86_64-apple-darwin")
    SET(CONFIG_C_FLAGS "")
    SET(CONFIG_LDFLAGS "")
  ELSE(OS_IS_64BIT)
    SET(CONFIG_HOST "i686-apple-darwin")
    SET(CONFIG_C_FLAGS "'-O3 -m32'")
    SET(CONFIG_LDFLAGS "-m32")
  ENDIF(OS_IS_64BIT)
ELSEIF(UNIX)
  # Unix
  IF(OS_IS_64BIT)
    SET(CONFIG_HOST "x86_64-pc-linux-gnu")
    SET(CONFIG_C_FLAGS "-fPIC")
    SET(CONFIG_LDFLAGS "")
  ELSE(OS_IS_64BIT)
    SET(CONFIG_HOST "i686-pc-linux-gnu")
    SET(CONFIG_C_FLAGS "'-fPIC -O3 -m32'")
    SET(CONFIG_LDFLAGS "-m32")
  ENDIF(OS_IS_64BIT)
ELSE(APPLE)
  # Something different
  MESSAGE(ERROR "Unhandled platform !!!")
ENDIF(APPLE)
# Add libjpeg-turbo
ExternalProject_Add(libjpeg_project
        GIT_REPOSITORY https://github.com/libjpeg-turbo/libjpeg-turbo.git
        # TIMEOUT 10
        # # Force separate output paths for debug and release builds to allow easy
        # # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
        # CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
        #            -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
        #            -Dgtest_force_shared_crt=ON
        #CMAKE_ARGS -G${CMAKE_GENERATOR} -DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE} -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE} -DPNG_SHARED=OFF -DPNG_TESTS=OFF
        #CMAKE_CACHE_ARGS -DCMAKE_C_FLAGS:STRING=-fPIC
        # Folder destination
        PREFIX ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libjpeg
        TMP_DIR ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libjpeg/tmp
        STAMP_DIR ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libjpeg/stamp
        DOWNLOAD_DIR ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libjpeg/download
        SOURCE_DIR ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libjpeg/src
        # generate configure
        UPDATE_COMMAND autoreconf -fiv
        # Configure step
        CONFIGURE_COMMAND ./configure --host ${CONFIG_HOST} CFLAGS=${CONFIG_C_FLAGS} LDFLAGS=${CONFIG_LDFLAGS} NASM=${NASM_PATH}
        # Build step
        BUILD_COMMAND make
        BUILD_IN_SOURCE 1
        # Disable install step
        INSTALL_COMMAND cmake -E echo "Skipping install step."
        # Wrap download, configure and build steps in a script to log output
        LOG_UPDATE ON
        LOG_DOWNLOAD ON
        LOG_CONFIGURE ON
        LOG_BUILD ON)

# Specify include dir
ExternalProject_Get_Property(libjpeg_project source_dir)
SET(PEG_INCLUDE_DIRS ${source_dir}/include PARENT_SCOPE)
# Library
ExternalProject_Get_Property(libjpeg_project binary_dir)
SET(LIBJPEG_LIBRARY_PATH ${binary_dir}/.libs/${CMAKE_STATIC_LIBRARY_PREFIX}jpeg.a)
ADD_LIBRARY(libjpeg STATIC IMPORTED GLOBAL)
SET_PROPERTY(TARGET libjpeg PROPERTY IMPORTED_LOCATION ${LIBJPEG_LIBRARY_PATH})
ADD_DEPENDENCIES(libjpeg libjpeg_project)
# Export libraries
SET(JPEG_LIBRARIES libjpeg PARENT_SCOPE)