set(SUBSYS_NAME dataset)
set(SUBSYS_DESC "FaceKit dataset helper library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS core io)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS} EXT_DEPS opencv)
if(build)  
  # Add sources 
  set(srcs
    src/augmentation_engine.cpp
    src/identity_cell.cpp
    src/flip_cell.cpp
    src/in_plane_rotation_cell.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/augmentation_engine.hpp
    include/facekit/${SUBSYS_NAME}/augmentation_cell.hpp
    include/facekit/${SUBSYS_NAME}/identity_cell.hpp
    include/facekit/${SUBSYS_NAME}/flip_cell.hpp
    include/facekit/${SUBSYS_NAME}/in_plane_rotation_cell.hpp)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add include folder location
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" FILES ${srcs} ${incs} LINK_WITH facekit_core facekit_io ${OpenCV_LIBS})

  #EXAMPLES
  IF(WITH_EXAMPLES)
    # Data Augmentation sample
    FACEKIT_ADD_EXAMPLE(data_augmentation FILES example/ex_augmentation_engine.cpp LINK_WITH facekit_core facekit_dataset)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  #FACEKIT_ADD_TEST(cmd_parser oglkit_test_cmd_parser FILES test/test_cmd_parser.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH oglkit_core)

  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
endif(build)
