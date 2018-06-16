# Based on: https://pabloariasal.github.io/2018/02/19/its-time-to-do-cmake-right/
include(GNUInstallDirs)

# ---[ Version
configure_file("${FACEKIT_SOURCE_DIR}/cmake/FaceKitConfigVersion.cmake.in"
               "${FACEKIT_BINARY_DIR}/FaceKitConfigVersion.cmake" @ONLY)
install(FILES "${FACEKIT_BINARY_DIR}/FaceKitConfigVersion.cmake"
        COMPONENT facekitconfig
        DESTINATION ${FACEKITCONFIG_INSTALL_DIR})
# ---[ Configuration
# Basically export all targets (i.e. library) with their proper include folder
# automatically. All targets are placed into a namespace LTS5
INSTALL(EXPORT facekit-export
        FILE FaceKitConfig.cmake
        NAMESPACE FaceKit::
        DESTINATION ${FACEKITCONFIG_INSTALL_DIR})