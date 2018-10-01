set(SUBSYS_NAME sim)
set(SUBSYS_DESC "FaceKit Simulation library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS core)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS})
if(build)  
  
  # Add sources 
  set(srcs
    src/box_generator.cpp
    src/euler_updater.cpp
    src/generator.cpp
    src/particles.cpp
    src/time_generator.cpp
    src/time_updater.cpp
    src/velocity_generator.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/box_generator.hpp
    include/facekit/${SUBSYS_NAME}/euler_updater.hpp
    include/facekit/${SUBSYS_NAME}/generator.hpp
    include/facekit/${SUBSYS_NAME}/particles.hpp
    include/facekit/${SUBSYS_NAME}/time_generator.hpp
    include/facekit/${SUBSYS_NAME}/time_updater.hpp
    include/facekit/${SUBSYS_NAME}/updater.hpp
    include/facekit/${SUBSYS_NAME}/velocity_generator.hpp)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" 
                      FILES ${srcs} ${incs} 
                      PUBLIC_LINK facekit_core)
  TARGET_INCLUDE_DIRECTORIES(${LIB_NAME} 
    PUBLIC 
      $<INSTALL_INTERFACE:include>
      $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

  #EXAMPLES
  IF(WITH_EXAMPLES)
    #FACEKIT_ADD_EXAMPLE(image_loader FILES example/ex_image_loader.cpp LINK_WITH facekit_core facekit_io)
  ENDIF(WITH_EXAMPLES)
  
  # TESTS
  
  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
endif(build)
