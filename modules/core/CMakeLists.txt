set(SUBSYS_NAME core)
set(SUBSYS_DESC "FaceKit core element library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS} EXT_DEPS opencv blas protobuf)
if(build)
  # Add sources 
  set(srcs
    src/allocator_factory.cpp
    src/allocator.cpp
    src/array_dims.cpp
    src/cmd_parser.cpp
    src/error.cpp
    src/linear_algebra.cpp
    src/logger.cpp
    src/memory.cpp
    src/string_util.cpp
    src/types.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/cmd_parser.hpp
    include/facekit/${SUBSYS_NAME}/enum_bitmask_operator.hpp
    include/facekit/${SUBSYS_NAME}/error.hpp
    include/facekit/${SUBSYS_NAME}/library_export.hpp
    include/facekit/${SUBSYS_NAME}/logger.hpp
    include/facekit/${SUBSYS_NAME}/refcounter.hpp
    include/facekit/${SUBSYS_NAME}/string_util.hpp
    include/facekit/${SUBSYS_NAME}/thread_pool.hpp
    include/facekit/${SUBSYS_NAME}/types.hpp)
  set(incs_math
    include/facekit/${SUBSYS_NAME}/math/linear_algebra.hpp
    include/facekit/${SUBSYS_NAME}/math/matrix.hpp
    include/facekit/${SUBSYS_NAME}/math/quaternion.hpp
    include/facekit/${SUBSYS_NAME}/math/type_comparator.hpp
    include/facekit/${SUBSYS_NAME}/math/vector.hpp)
  set(incs_mem
    include/facekit/${SUBSYS_NAME}/mem/allocator_factory.hpp
    include/facekit/${SUBSYS_NAME}/mem/allocator.hpp
    include/facekit/${SUBSYS_NAME}/mem/memory.hpp)
  set(proto
    src/proto/array_dims.proto)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add include folder location
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" 
                      FILES ${srcs} ${incs} ${incs_math} ${incs_mem} 
                      PROTO_FILES ${proto} 
                      LINK_WITH ${OpenCV_LIBRARIES} ${BLAS_LIBRARIES} ${Protobuf_LIBRARIES})

  #EXAMPLES
  IF(WITH_EXAMPLES)
      #LTS5_ADD_EXAMPLE(facekit_video_splitter FILES file LINK_WITH lib)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  FACEKIT_ADD_TEST(ut_cmd_parser cmd_parser FILES test/ut_cmd_parser.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_linear_algebra linear_algebra FILES test/ut_linear_algebra.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core ${OpenCV_LIBRARIES})
  FACEKIT_ADD_TEST(ut_logger logger FILES test/ut_logger.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_refcounter refcounter FILES test/ut_refcounter.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "")
  FACEKIT_ADD_TEST(ut_types types FILES test/ut_types.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_allocator allocator FILES test/ut_allocator.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_array_dims array_dims FILES test/ut_array_dims.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core ${Protobuf_LIBRARIES})

  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/math" ${incs_math})
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/mem" ${incs_mem})
endif(build)
