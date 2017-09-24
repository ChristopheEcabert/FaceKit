set(SUBSYS_NAME geometry)
set(SUBSYS_DESC "FaceKit geometry processing library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS core)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS} EXT_DEPS "")
if(build)
  # Add sources 
  set(srcs
    src/mesh.cpp)
  set(srcs_ext
    ${FACEKIT_SOURCE_DIR}/3rdparty/ply/plyfile.c)
  set(incs
    include/facekit/${SUBSYS_NAME}/aabb.hpp
    include/facekit/${SUBSYS_NAME}/mesh.hpp)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add include folder location
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${FACEKIT_SOURCE_DIR}/3rdparty)

  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" FILES ${srcs} ${incs} ${srcs_ext})

  #EXAMPLES
  IF(WITH_EXAMPLES)
      #FACEKIT_ADD_EXAMPLE(oglkit_video_splitter FILES file LINK_WITH lib)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  #FACEKIT_ADD_TEST(cmd_parser oglkit_test_cmd_parser FILES test/test_cmd_parser.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH oglkit_core)

  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
endif(build)
