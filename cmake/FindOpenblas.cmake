# - Finds Openblas and all dependencies
# Once done this will define
#  OPENBLAS_FOUND- System has Openblas
#  OPENBLAS_INCLUDE_DIR- The Openblas include directories
#  OPENBLAS_LIB- The libraries needed to use Openblas

MESSAGE(STATUS "Look for openblas")

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

# Look for includes
FIND_PATH(OPENBLAS_INCLUDE_DIR NAMES cblas.h HINTS ${Open_BLAS_INCLUDE_SEARCH_PATHS})
# Library
IF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  FIND_LIBRARY(OPENBLAS_LIB NAMES libopenblas openblas HINTS ${Open_BLAS_LIB_SEARCH_PATHS})
ELSE(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")
  FIND_FILE(OPENBLAS_LIBRARIES libopenblas.dll.a ${Open_BLAS_LIB_SEARCH_PATHS})
ENDIF(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Windows")

# handle the QUIETLY and REQUIRED arguments and set FlyCapture_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( "FindPackageHandleStandardArgs" )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "BLAS" DEFAULT_MSG OPENBLAS_INCLUDE_DIR OPENBLAS_LIBRARIES)
MARK_AS_ADVANCED(OPENBLAS_INCLUDE_DIR)
IF(BLAS_FOUND AND NOT TARGET ext::blas)
  # Dump info
  MESSAGE(STATUS "Found BLAS libraries: ${OPENBLAS_LIB}")
  MESSAGE(STATUS "Found BLAS include: ${OPENBLAS_INCLUDE_DIR}")
  # Define target
  ADD_LIBRARY(ext::blas SHARED IMPORTED)
  SET_TARGET_PROPERTIES(ext::blas PROPERTIES INCLUDE_DIRECTORIES "${OPENBLAS_INCLUDE_DIR}")
  SET_TARGET_PROPERTIES(ext::blas PROPERTIES IMPORTED_LOCATION "${OPENBLAS_LIBRARIES}")
ENDIF(BLAS_FOUND AND NOT TARGET ext::blas)