# - Finds Openblas and all dependencies
# Once done this will define
#  OPENBLAS_FOUND- System has Openblas
#  OPENBLAS_INCLUDE_DIR- The Openblas include directories
#  OPENBLAS_LIB- The libraries needed to use Openblas

MESSAGE(STATUS "Look for openblas")

FILE(TO_CMAKE_PATH "$ENV{OpenBLAS_HOME}" OpenBLAS_WIN_ROOT)
SET(OPENBLAS_INCLUDE_SEARCH_PATHS /usr/include
                                  /usr/include/openblas-base
                                  /usr/local/include
                                  /usr/local/include/openblas-base
                                  /opt/OpenBLAS/include
                                  $ENV{OPENBLAS_ROOT}/include
                                  $ENV{OpenBLAS_WIN_ROOT}
                                  $ENV{OpenBLAS_WIN_ROOT}/include)
SET(OPENBLAS_LIB_SEARCH_PATHS /lib/
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
                              $ENV{OpenBLAS_WIN_ROOT}/lib)
SET(OPENBLAS_NAMES libopenblas openblas libopenblas.dll)
# Look for includes
FIND_PATH(OPENBLAS_INCLUDE_DIR NAMES cblas.h HINTS ${OPENBLAS_INCLUDE_SEARCH_PATHS})
# Library
FIND_LIBRARY(OPENBLAS_LIBRARIES NAMES ${OPENBLAS_NAMES} HINTS ${OPENBLAS_LIB_SEARCH_PATHS})

# handle the QUIETLY and REQUIRED arguments and set BLAS_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE( "FindPackageHandleStandardArgs" )
FIND_PACKAGE_HANDLE_STANDARD_ARGS( "BLAS" DEFAULT_MSG OPENBLAS_INCLUDE_DIR OPENBLAS_LIBRARIES)
MARK_AS_ADVANCED(OPENBLAS_INCLUDE_DIR)
IF(BLAS_FOUND AND NOT TARGET ext::blas)
  # Dump info
  MESSAGE(STATUS "Found BLAS libraries: ${OPENBLAS_LIBRARIES}")
  MESSAGE(STATUS "Found BLAS include: ${OPENBLAS_INCLUDE_DIR}")
  # Define target
  ADD_LIBRARY(ext::blas SHARED IMPORTED)
  SET_TARGET_PROPERTIES(ext::blas PROPERTIES INCLUDE_DIRECTORIES "${OPENBLAS_INCLUDE_DIR}")
  SET_TARGET_PROPERTIES(ext::blas PROPERTIES IMPORTED_LOCATION "${OPENBLAS_LIBRARIES}")
ENDIF(BLAS_FOUND AND NOT TARGET ext::blas)