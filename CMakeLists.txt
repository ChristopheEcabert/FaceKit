### ---[ FACEKIT global CMake
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

if(POLICY CMP0017)
  # Do not include files in CMAKE_MODULE_PATH from files
  # in CMake module directory. Fix MXE build
  #cmake_policy(SET CMP0017 NEW)
endif()

if(POLICY CMP0020 AND (WIN32 AND NOT MINGW))
  cmake_policy(SET CMP0020 NEW) # Automatically link Qt executables to qtmain target on Windows
endif()

if(POLICY CMP0048)
  cmake_policy(SET CMP0048 OLD) # do not use VERSION option in project() command
endif()

if(POLICY CMP0054)
  cmake_policy(SET CMP0054 OLD) # Silent warnings about quoted variables
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "possible configurations" FORCE)

# In case the user does not setup CMAKE_BUILD_TYPE, assume it's RelWithDebInfo
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "build type default to Release" FORCE)
endif("${CMAKE_BUILD_TYPE}" STREQUAL "")

project(FACEKIT)
string(TOLOWER ${PROJECT_NAME} PROJECT_NAME_LOWER)

### ---[ Find universal dependencies
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/" ${CMAKE_MODULE_PATH})

# ---[ Include pkgconfig
include (FindPkgConfig)

# ---[ Release/Debug specific flags
if(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  add_definitions("-DEIGEN_NO_DEBUG")
endif()

# ---[ special maintainer mode
SET(CMAKE_CXX_FLAGS_MAINTAINER "-pedantic -Wno-variadic-macros -Weffc++ -Wno-long-long" CACHE STRING
    "Flags used by the C++ compiler during maintainer builds."
    FORCE)
SET(CMAKE_C_FLAGS_MAINTAINER "-pedantic -Wno-variadic-macros -Weffc++ -Wno-long-long" CACHE STRING
    "Flags used by the C compiler during maintainer builds."
    FORCE)
SET(CMAKE_EXE_LINKER_FLAGS_MAINTAINER
    "-Wl,--warn-unresolved-symbols,--warn-once" CACHE STRING
    "Flags used for linking binaries during maintainer builds."
    FORCE)
SET(CMAKE_SHARED_LINKER_FLAGS_MAINTAINER
    "-Wl,--warn-unresolved-symbols,--warn-once" CACHE STRING
    "Flags used by the shared libraries linker during maintainer builds."
    FORCE)
MARK_AS_ADVANCED(
    CMAKE_CXX_FLAGS_MAINTAINER
    CMAKE_C_FLAGS_MAINTAINER
    CMAKE_EXE_LINKER_FLAGS_MAINTAINER
    CMAKE_SHARED_LINKER_FLAGS_MAINTAINER )
# Update the documentation string of CMAKE_BUILD_TYPE for GUIs
SET(CMAKE_BUILD_TYPE "${CMAKE_BUILD_TYPE}" CACHE STRING
    "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel Maintainer."
    FORCE)

# Compiler identification
# Define a variable CMAKE_COMPILER_IS_X where X is the compiler short name.
# Note: CMake automatically defines one for GNUCXX, nothing to do in this case.
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang"  OR CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
  set(CMAKE_COMPILER_IS_CLANG 1)
elseif(__COMPILER_PATHSCALE)
  set(CMAKE_COMPILER_IS_PATHSCALE 1)
elseif(MSVC)
  set(CMAKE_COMPILER_IS_MSVC 1)
endif()

include("${FACEKIT_SOURCE_DIR}/cmake/facekit_targets.cmake")
include("${FACEKIT_SOURCE_DIR}/cmake/facekit_options.cmake")

# ---[ Unix/Darwin/Windows specific flags
if(CMAKE_COMPILER_IS_GNUCXX)
  if("${CMAKE_CXX_FLAGS}" STREQUAL "")
    SET(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unknown-pragmas -fno-strict-aliasing -Wno-format-extra-args -Wno-sign-compare -Wno-invalid-offsetof -Wno-conversion -std=c++11 ${SSE_FLAGS}")

    # Enable -Wabi for GCC > 4.3, and -Wno-deprecated for GCC < 4.3
    # to disable a lot of warnings which are not fixable
    execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (GCC_VERSION VERSION_GREATER 4.3)
      message(STATUS "-- GCC > 4.3 found, enabling -Wabi")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wabi")
    else()
      message(STATUS "-- GCC < 4.3 found, enabling -Wno-deprecated")
      SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
    endif ()
  endif()

  if(NOT ANDROID)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
  endif(NOT ANDROID)

  if("${CMAKE_SHARED_LINKER_FLAGS}" STREQUAL "" AND NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    SET(CMAKE_SHARED_LINKER_FLAGS "-Wl,--as-needed")
  endif()

  if(WIN32)
    if(FACEKIT_SHARED_LIBS)
      SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--export-all-symbols -Wl,--enable-auto-import")
      if (MINGW)
        add_definitions("-DBOOST_THREAD_USE_LIB")
        SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--allow-multiple-definition")
      endif()
    else(FACEKIT_SHARED_LIBS)
      add_definitions("-DBOOST_LIB_DIAGNOSTIC -DBOOST_THREAD_USE_LIB")
    endif(FACEKIT_SHARED_LIBS)
  endif()
endif()

if(CMAKE_COMPILER_IS_MSVC)
  add_definitions("-DBOOST_ALL_NO_LIB -D_SCL_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_WARNINGS -DNOMINMAX -DFACEKIT_ONLY_CORE_POINT_TYPES /bigobj ${SSE_DEFINITIONS}")
  if("${CMAKE_CXX_FLAGS}" STREQUAL " /DWIN32 /D_WINDOWS /W3 /GR /EHsc")	# Check against default flags
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj /EHsc /fp:precise /wd4800 /wd4521 /wd4251 /wd4275 /wd4305 /wd4355 ${SSE_FLAGS}")

    # Add extra code generation/link optimizations
    if(CMAKE_MSVC_CODE_LINK_OPTIMIZATION)
      SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /GL")
      SET(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /LTCG /OPT:REF")
      SET(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")
    endif(CMAKE_MSVC_CODE_LINK_OPTIMIZATION)
    # /MANIFEST:NO") # please, don't disable manifest generation, otherwise crash at start for vs2008

    if( MSVC_VERSION GREATER 1500 AND ${CMAKE_VERSION} VERSION_GREATER "2.8.6")
      include(ProcessorCount)
      ProcessorCount(N)
      if(NOT N EQUAL 0)
        SET(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   /MP${N} ")
        SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP${N} ")
      endif()
    endif()
  endif()
endif()

if (CMAKE_COMPILER_IS_PATHSCALE)
  if("${CMAKE_CXX_FLAGS}" STREQUAL "")
    SET(CMAKE_CXX_FLAGS "-Wno-uninitialized -zerouv -pthread -mp")
  endif()
endif()

if (CMAKE_COMPILER_IS_CLANG)
  if("${CMAKE_C_FLAGS}" STREQUAL "")
    SET(CMAKE_C_FLAGS "-Qunused-arguments")
  endif()
  if("${CMAKE_CXX_FLAGS}" STREQUAL "")
    #Unfortunately older Clang versions do not have this: -Wno-unnamed-type-template-args
    SET(CMAKE_CXX_FLAGS "-ftemplate-depth=1024 -Qunused-arguments -Wno-invalid-offsetof -std=c++11 -stdlib=libc++ ${SSE_FLAGS}") 
  endif()
  SET(CLANG_LIBRARIES "c++")
endif()

include("${FACEKIT_SOURCE_DIR}/cmake/facekit_utils.cmake")
set(FACEKIT_VERSION 1.0.0 CACHE STRING "FaceKit version")
DISSECT_VERSION()
GET_OS_INFO()
SET_INSTALL_DIRS()

set(FACEKIT_OUTPUT_LIB_DIR "${FACEKIT_BINARY_DIR}/${LIB_INSTALL_DIR}")
set(FACEKIT_OUTPUT_BIN_DIR "${FACEKIT_BINARY_DIR}/${BIN_INSTALL_DIR}")
set(FACEKIT_OUTPUT_3RDPARTY_LIB_DIR "${FACEKIT_BINARY_DIR}/3rdparty")
make_directory("${FACEKIT_OUTPUT_LIB_DIR}")
make_directory("${FACEKIT_OUTPUT_BIN_DIR}")
make_directory("${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}")
if(WIN32)
  foreach(config ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${config} CONFIG)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG} "${FACEKIT_OUTPUT_LIB_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG} "${FACEKIT_OUTPUT_BIN_DIR}")
    # ---[ Windows requires DLLs (shared libraries) to be installed in the same directory as executables
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG} "${FACEKIT_OUTPUT_BIN_DIR}")
  endforeach(config)
else(WIN32)
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${FACEKIT_OUTPUT_LIB_DIR}")
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${FACEKIT_OUTPUT_BIN_DIR}")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${FACEKIT_OUTPUT_LIB_DIR}")
endif(WIN32)

# Add an "uninstall" target
configure_file("${FACEKIT_SOURCE_DIR}/cmake/uninstall_target.cmake.in"
               "${FACEKIT_BINARY_DIR}/uninstall_target.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P
                  "${FACEKIT_BINARY_DIR}/uninstall_target.cmake")

###
# this is copy paste form http://www.itk.org/Wiki/CMake_RPATH_handling
# in order to always make a full statement RPATH
###
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    SET(CMAKE_MACOSX_RPATH ON)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}" is_system_dir)
IF("${is_system_dir}" STREQUAL "-1")
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR}")
ENDIF("${is_system_dir}" STREQUAL "-1")

### ---[ Find universal dependencies
# the gcc-4.2.1 coming with MacOS X is not compatible with the OpenMP pragmas we use, so disabling OpenMP for it
if((NOT APPLE) OR (NOT CMAKE_COMPILER_IS_GNUCXX) OR (GCC_VERSION VERSION_GREATER 4.2.1) OR (NOT CMAKE_COMPILER_IS_CLANG))
  FIND_PACKAGE(OpenMP)
endif()
if(OPENMP_FOUND)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  message (STATUS "Found OpenMP")
  if(MSVC)
    if(MSVC_VERSION EQUAL 1500)
      set(OPENMP_DLL VCOMP90)
    elseif(MSVC_VERSION EQUAL 1600)
      set(OPENMP_DLL VCOMP100)
    elseif(MSVC_VERSION EQUAL 1700)
      set(OPENMP_DLL VCOMP110)
    elseif(MSVC_VERSION EQUAL 1800)
      set(OPENMP_DLL VCOMP120)
    elseif(MSVC_VERSION EQUAL 1900)
      set(OPENMP_DLL VCOMP140)
    elseif(MSVC_VERSION EQUAL 1910)
      set(OPENMP_DLL VCOMP140)
    endif()
    if(OPENMP_DLL)
      set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} /DELAYLOAD:${OPENMP_DLL}D.dll")
      set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} /DELAYLOAD:${OPENMP_DLL}.dll")
    else(OPENMP_DLL)
      message(WARNING "Delay loading flag for OpenMP DLL is invalid.")
    endif(OPENMP_DLL)
  endif(MSVC)
else(OPENMP_FOUND)
  message (STATUS "Not found OpenMP")
endif()

### ---[ BLAS / LAPACK
# Check platform
IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  FIND_PACKAGE(BLAS REQUIRED)
  FIND_PACKAGE(LAPACK REQUIRED)
  IF(BLAS_FOUND AND LAPACK_FOUND)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${BLAS_LINKER_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LAPACK_LINKER_FLAGS}")
  ENDIF(BLAS_FOUND AND LAPACK_FOUND)
ELSE(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  # OpenBlas
  FIND_PACKAGE(Openblas REQUIRED)
  IF(OPENBLAS_FOUND)
    INCLUDE_DIRECTORIES(${OPENBLAS_INCLUDE_DIR})
    SET(BLAS_LIBRARIES ${OPENBLAS_LIB})
    SET(LAPACK_LIBRARIES ${OPENBLAS_LIB})
    SET(BLAS_FOUND TRUE)
    SET(LAPACK_FOUND TRUE)
  ENDIF(OPENBLAS_FOUND)
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")

### ---[ OpenCV
# Temporary fix for OPENCV
# http://stackoverflow.com/questions/37534604/opencv-with-cmake-version-3-5-2-vs-cmake-2-x-x
SET(CUDA_USE_STATIC_CUDA_RUNTIME OFF CACHE BOOL "Cuda runtime" FORCE)
FIND_PACKAGE(OpenCV REQUIRED)
IF(OpenCV_LIBRARIES)
  INCLUDE_DIRECTORIES(${OpenCV_INCLUDE_DIRS})
	SET(OPENCV_FOUND TRUE)
	MARK_AS_ADVANCED(OpenCV_DIR)
    # Remove, -lopencv_dep_cudart options on OSX
    IF(APPLE AND CMAKE_MINOR_VERSION GREATER 6)
        FOREACH(_lib ${OpenCV_LIBS})
            GET_PROPERTY(OCV_IMPORT_INTERFACE TARGET ${_lib} PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE)
            FOREACH(_item ${OCV_IMPORT_INTERFACE})
                IF(NOT ${_item} MATCHES "opencv_dep_cudart")
                    SET(OCV_IMPORT_INTERFACE_CLEAN ${OCV_IMPORT_INTERFACE_CLEAN} ${_item})
                ENDIF(NOT ${_item} MATCHES "opencv_dep_cudart")
            ENDFOREACH(_item)
            SET_TARGET_PROPERTIES(${_lib} PROPERTIES IMPORTED_LINK_INTERFACE_LIBRARIES_RELEASE "${OCV_IMPORT_INTERFACE_CLEAN}")
        ENDFOREACH(_lib)
    ENDIF(APPLE AND CMAKE_MINOR_VERSION GREATER 6)
ENDIF(OpenCV_LIBRARIES)

### ---[ Doxygen
FIND_PACKAGE(Doxygen QUIET)


### ---[ Create the config.h file
set(facekit_config_h_in "${CMAKE_CURRENT_SOURCE_DIR}/cmake/facekit_config.h.in")
set(facekit_config_h "${CMAKE_CURRENT_BINARY_DIR}/include/facekit/facekit_config.h")
configure_file("${facekit_config_h_in}" "${facekit_config_h}")
FACEKIT_ADD_INCLUDES("" "" "${facekit_config_h}")
include_directories("${CMAKE_CURRENT_BINARY_DIR}/include")

### ---[ Set up for tests
IF(WITH_TESTS)
  ENABLE_TESTING()
ENDIF(WITH_TESTS)

### ---[ Documentation
IF(WITH_DOCS)
  ADD_SUBDIRECTORY(modules/doc)
ENDIF(WITH_DOCS)

### ---[ Add the libraries subdirectories (i.e. modules)
collect_subproject_directory_names("${FACEKIT_SOURCE_DIR}/modules" "CMakeLists.txt" FACEKIT_MODULES_NAMES FACEKIT_MODULES_DIRS doc)
set(FACEKIT_MODULES_NAMES_UNSORTED ${FACEKIT_MODULES_NAMES})
topological_sort(FACEKIT_MODULES_NAMES FACEKIT_ _DEPENDS)
sort_relative(FACEKIT_MODULES_NAMES_UNSORTED FACEKIT_MODULES_NAMES FACEKIT_MODULES_DIRS)
foreach(subdir ${FACEKIT_MODULES_DIRS})
  add_subdirectory("${FACEKIT_SOURCE_DIR}/modules/${subdir}")
endforeach(subdir)

### ---[ Configure FACEKITConfig.cmake
include("${FACEKIT_SOURCE_DIR}/cmake/facekit_config.cmake")

### ---[ Finish up
FACEKIT_WRITE_STATUS_REPORT()
FACEKIT_RESET_MAPS()

