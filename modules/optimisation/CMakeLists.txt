set(SUBSYS_NAME optimisation)
set(SUBSYS_DESC "FaceKit Optimisation library")
#Set internal library dependencies, here there isn't other dependencies
set(SUBSYS_DEPS core)

set(build TRUE)
FACEKIT_SUBSYS_OPTION(build "${SUBSYS_NAME}" "${SUBSYS_DESC}" ON)
#Add dependencies as well as external dependencies
FACEKIT_SUBSYS_DEPEND(build "${SUBSYS_NAME}" DEPS ${SUBSYS_DEPS})
if(build)  

  
  # Add sources 
  set(srcs
    src/genetic_solver
    src/population.cpp)
  set(incs
    include/facekit/${SUBSYS_NAME}/chromosome.hpp
    include/facekit/${SUBSYS_NAME}/genetic_solver.hpp
    include/facekit/${SUBSYS_NAME}/population.hpp)
  # Set library name
  set(LIB_NAME "facekit_${SUBSYS_NAME}")
  # Add include folder location
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
  # Add library
  FACEKIT_ADD_LIBRARY("${LIB_NAME}" "${SUBSYS_NAME}" FILES ${srcs} ${incs} LINK_WITH facekit_core)
   
  #EXAMPLES
  IF(WITH_EXAMPLES)
    FACEKIT_ADD_EXAMPLE(genetic_solver FILES example/ex_genetic_solver.cpp LINK_WITH facekit_core facekit_optimisation)
  ENDIF(WITH_EXAMPLES)

  # TESTS
  
  # Install include files
  FACEKIT_ADD_INCLUDES("${SUBSYS_NAME}" "${SUBSYS_NAME}" ${incs})
endif(build)
