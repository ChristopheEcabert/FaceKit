set(SUBSYS_NAME core)
set(SUBSYS_DESC "FaceKit core element library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS} EXT_DEPS "")
if(build)
  # Add sources 
  set(srcs
    src/cmd_parser.cpp
    src/error.cpp
    src/string_util.cpp
    src/cmd_parser.cpp)

  set(incs
    include/facekit/${SUBSYS_NAME}/cmd_parser.hpp
    include/facekit/${SUBSYS_NAME}/error.hpp
    include/facekit/${SUBSYS_NAME}/library_export.hpp)
  set(incs_math
    include/facekit/${SUBSYS_NAME}/math/matrix.hpp
    include/facekit/${SUBSYS_NAME}/math/quaternion.hpp
    include/facekit/${SUBSYS_NAME}/math/type_comparator.hpp
    include/facekit/${SUBSYS_NAME}/math/vector.hpp)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add include folder location
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" FILES ${srcs} ${incs} ${incs_math})

  #EXAMPLES
  IF(WITH_EXAMPLES)
      #LTS5_ADD_EXAMPLE(facekit_video_splitter FILES file LINK_WITH lib)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  FACEKIT_ADD_TEST(cmd_parser facekit_test_cmd_parser FILES test/test_cmd_parser.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)

  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/math" ${incs_math})
endif(build)
