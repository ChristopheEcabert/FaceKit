# - Finds Openblas and all dependencies
# Once done this will define
#  OPENBLAS_FOUND- System has Openblas
#  OPENBLAS_INCLUDE_DIR- The Openblas include directories
#  OPENBLAS_LIB- The libraries needed to use Openblas

MESSAGE(STATUS "Look for openblas")
MESSAGE(STATUS "OpenBLAS env root: $ENV{OPENBLAS_ROOT}")

FILE(TO_CMAKE_PATH "$ENV{OpenBLAS_HOME}" OpenBLAS_WIN_ROOT)
SET(Open_BLAS_INCLUDE_SEARCH_PATHS
  /usr/include
  /usr/include/openblas-base
  /usr/local/include
  /usr/local/include/openblas-base
  /opt/OpenBLAS/include
  $ENV{OPENBLAS_ROOT}/include
  $ENV{OpenBLAS_WIN_ROOT}
  $ENV{OpenBLAS_WIN_ROOT}/include
)

SET(Open_BLAS_LIB_SEARCH_PATHS
        /lib/
        /lib/openblas-base
        /lib64/
        /usr/lib
        /usr/lib/openblas-base
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
        /opt/OpenBLAS/lib
        $ENV{OpenBLAS}
        $ENV{OPENBLAS_ROOT}/lib
        $ENV{OpenBLAS_WIN_ROOT}/lib
        $ENV{OpenBLAS_WIN_ROOT}
        $ENV{OpenBLAS_WIN_ROOT}/lib
 )

FIND_PATH(OPENBLAS_INCLUDE_DIR NAMES cblas.h HINTS ${Open_BLAS_INCLUDE_SEARCH_PATHS})
FIND_LIBRARY(OPENBLAS_LIB NAMES libopenblas openblas HINTS ${Open_BLAS_LIB_SEARCH_PATHS})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    find_file(OPENBLAS_LIB libopenblas.dll.a ${Open_BLAS_LIB_SEARCH_PATHS})
ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
#MESSAGE("Lib file : ${OPENBLAS_LIB}")

SET(OPENBLAS_FOUND TRUE)

#    Check include files
IF(NOT OPENBLAS_INCLUDE_DIR)
    SET(OPENBLAS_FOUND FALSE)
    MESSAGE(STATUS "Could not find OPENBLAS include. Turning OpenBLAS_FOUND off")
ENDIF()

#    Check libraries
IF(NOT OPENBLAS_LIB)
    SET(OPENBLAS_FOUND FALSE)
    MESSAGE(STATUS "Could not find OpenBLAS lib. Turning OpenBLAS_FOUND off")
ENDIF()

IF (OPENBLAS_FOUND)
  IF (NOT OPENBLAS_FIND_QUIETLY)
    MESSAGE(STATUS "Found OpenBLAS libraries: ${OPENBLAS_LIB}")
    MESSAGE(STATUS "Found OpenBLAS include: ${OPENBLAS_INCLUDE_DIR}")
  ENDIF (NOT OPENBLAS_FIND_QUIETLY)
ELSE (OPENBLAS_FOUND)
  IF (OPENBLAS_FIND_REQUIRED)
    MESSAGE(FATAL_ERROR "Could not find OpenBLAS")
  ENDIF (OPENBLAS_FIND_REQUIRED)
ENDIF (OPENBLAS_FOUND)

MARK_AS_ADVANCED(
    OPENBLAS_INCLUDE_DIR
    OPENBLAS_LIB
    OPENBLAS_FOUND
)