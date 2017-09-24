set(SUBSYS_NAME model)
set(SUBSYS_DESC "FACEKIT face model")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS core io geometry)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS} EXT_DEPS opencv)

if(build)
  set(srcs
    src/pca_model.cpp
    src/pca_model_factory.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/pca_model_factory.hpp
    include/facekit/${SUBSYS_NAME}/pca_model.hpp)

  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

  # Define library SRC, LINK
  SET(LIB_SRCS ${srcs} ${incs})
  SET(LIB_LINK ${OpenCV_LIBS} facekit_core facekit_io)

  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" FILES ${LIB_SRCS})
  target_link_libraries("${LIB_NAME}" ${LIB_LINK})

  #EXAMPLE
  IF(OPT_BUILD_EXAMPLE)
  ENDIF(OPT_BUILD_EXAMPLE)

  ## Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
endif(build)
