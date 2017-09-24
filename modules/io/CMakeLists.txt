set(SUBSYS_NAME io)
set(SUBSYS_DESC "FaceKit I/O library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS core)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS} EXT_DEPS opencv)
if(build)  
  # Add sources 
  set(srcs
    src/file_io.cpp
    src/object_header.cpp
    src/object_manager.cpp
    src/object_proxy.cpp
    src/serializable.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/file_io.hpp
    include/facekit/${SUBSYS_NAME}/object_header.hpp
    include/facekit/${SUBSYS_NAME}/object_manager.hpp
    include/facekit/${SUBSYS_NAME}/object_proxy.hpp
    include/facekit/${SUBSYS_NAME}/serializable.hpp)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add include folder location
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" FILES ${srcs} ${incs} LINK_WITH facekit_core ${OpenCV_LIBS})

  #EXAMPLES
  IF(WITH_EXAMPLES)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  #FACEKIT_ADD_TEST(cmd_parser oglkit_test_cmd_parser FILES test/test_cmd_parser.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH oglkit_core)

  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
endif(build)
