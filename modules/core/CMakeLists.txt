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
    src/cmd_parser.cpp
    src/error.cpp
    src/file_system_factory.cpp
    src/file_system.cpp
    src/linear_algebra.cpp
    src/logger.cpp
    src/map_allocator.cpp
    src/memory.cpp
    src/nd_array_dims.cpp
    src/nd_array.cpp
    src/posix_file_system.cpp
    src/proto.cpp
    src/scanner.cpp
    src/status.cpp
    src/string.cpp
    src/types.cpp
    src/windows_file_system.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/cmd_parser.hpp
    include/facekit/${SUBSYS_NAME}/error.hpp
    include/facekit/${SUBSYS_NAME}/library_export.hpp
    include/facekit/${SUBSYS_NAME}/logger.hpp
    include/facekit/${SUBSYS_NAME}/nd_array_dims.hpp
    include/facekit/${SUBSYS_NAME}/nd_array_map.hpp
    include/facekit/${SUBSYS_NAME}/nd_array.hpp
    include/facekit/${SUBSYS_NAME}/nd_array.inl.hpp
    include/facekit/${SUBSYS_NAME}/refcounter.hpp
    include/facekit/${SUBSYS_NAME}/status.hpp
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
    include/facekit/${SUBSYS_NAME}/mem/map_allocator.hpp
    include/facekit/${SUBSYS_NAME}/mem/memory.hpp)
  set(incs_sys
    include/facekit/${SUBSYS_NAME}/sys/file_system_factory.hpp
    include/facekit/${SUBSYS_NAME}/sys/file_system.hpp
    include/facekit/${SUBSYS_NAME}/sys/posix_file_system.hpp
    include/facekit/${SUBSYS_NAME}/sys/windows_file_system.hpp)
  set(incs_utils
    include/facekit/${SUBSYS_NAME}/utils/enum_bitmask_operator.hpp
    include/facekit/${SUBSYS_NAME}/utils/proto.hpp
    include/facekit/${SUBSYS_NAME}/utils/scanner.hpp
    include/facekit/${SUBSYS_NAME}/utils/string.hpp)
  set(proto
    src/proto/nd_array_dims.proto
    src/proto/nd_array.proto
    src/proto/types.proto)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}"
                      FILES ${srcs} ${incs} ${incs_math} ${incs_mem} ${incs_utils}
                      PROTO_FILES ${proto}
                      PUBLIC_LINK ${OpenCV_LIBRARIES} 
                      PRIVATE_LINK ${BLAS_LIBRARIES} ${Protobuf_LIBRARIES})
  TARGET_INCLUDE_DIRECTORIES(${LIB_NAME} 
    PUBLIC 
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/include)
  #EXAMPLES
  IF(WITH_EXAMPLES)
    FACEKIT_ADD_EXAMPLE(thread_pool FILES example/ex_thread_pool.cpp LINK_WITH facekit_core)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  FACEKIT_ADD_TEST(ut_cmd_parser cmd_parser FILES test/ut_cmd_parser.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_linear_algebra linear_algebra FILES test/ut_linear_algebra.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_logger logger FILES test/ut_logger.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_refcounter refcounter FILES test/ut_refcounter.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_types types FILES test/ut_types.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_status status FILES test/ut_status.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_allocator allocator FILES test/ut_allocator.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_nd_array_dims nd_array_dims FILES test/ut_nd_array_dims.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core ${Protobuf_LIBRARIES})
  FACEKIT_ADD_TEST(ut_nd_array_map nd_array_map FILES test/ut_nd_array_map.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_nd_array nd_array FILES test/ut_nd_array.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_paths paths FILES test/ut_path.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_filesystem posix_file_system FILES test/ut_file_system.cpp WORKING_DIR "${FACEKIT_BINARY_DIR}" ARGUMENTS "" LINK_WITH facekit_core)
  FACEKIT_ADD_TEST(ut_scanner scanner FILES test/ut_scanner.cpp WORKING_DIR "${FACEKIT_BINARY_DIR}" ARGUMENTS "" LINK_WITH facekit_core)

  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/math" ${incs_math})
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/mem" ${incs_mem})
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/sys" ${incs_sys})
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}/utils" ${incs_utils})
endif(build)
