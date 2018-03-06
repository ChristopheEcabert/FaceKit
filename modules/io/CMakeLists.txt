set(SUBSYS_NAME io)
set(SUBSYS_DESC "FaceKit I/O library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS core)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS} EXT_DEPS opencv)
if(build)  
  # Add 3rdparty
  ADD_SUBDIRECTORY(${FACEKIT_SOURCE_DIR}/3rdparty/libpng ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libpng EXCLUDE_FROM_ALL)
  ADD_SUBDIRECTORY(${FACEKIT_SOURCE_DIR}/3rdparty/libjpeg ${FACEKIT_OUTPUT_3RDPARTY_LIB_DIR}/libjpeg EXCLUDE_FROM_ALL)
  
  # Add sources 
  set(srcs
    src/file_io.cpp
    src/image_factory.cpp
    src/image.cpp
    src/jpeg_image.cpp
    src/object_header.cpp
    src/object_manager.cpp
    src/object_proxy.cpp
    src/png_image.cpp
    src/serializable.cpp
    src/tga_image.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/file_io.hpp
    include/facekit/${SUBSYS_NAME}/image_factory.hpp
    include/facekit/${SUBSYS_NAME}/image.hpp
    include/facekit/${SUBSYS_NAME}/jpeg_image.hpp
    include/facekit/${SUBSYS_NAME}/object_header.hpp
    include/facekit/${SUBSYS_NAME}/object_manager.hpp
    include/facekit/${SUBSYS_NAME}/object_proxy.hpp
    include/facekit/${SUBSYS_NAME}/png_image.hpp
    include/facekit/${SUBSYS_NAME}/serializable.hpp
    include/facekit/${SUBSYS_NAME}/tga_image.hpp)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add include folder location
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include 
                      ${FACEKIT_SOURCE_DIR}/3rdparty
                      ${FACEKIT_OUTPUT_PROTO_DIR}
                      ${PNG_INCLUDE_DIRS}
                      ${JPEG_INCLUDE_DIRS})
  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" FILES ${srcs} ${incs} LINK_WITH facekit_core ${OpenCV_LIBS} ${PNG_LIBRARIES} ${JPEG_LIBRARIES})
  ADD_DEPENDENCIES("${LIB_NAME}" libpng libjpeg)
  #EXAMPLES
  IF(WITH_EXAMPLES)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  #FACEKIT_ADD_TEST(cmd_parser oglkit_test_cmd_parser FILES test/test_cmd_parser.cpp WORKING_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test" ARGUMENTS "" LINK_WITH oglkit_core)

  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
endif(build)
