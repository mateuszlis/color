INCLUDE(InstallRequiredSystemLibraries)

#SET(CPACK_GENERATOR DMG)
SET(CPACK_PACKAGE_NAME "color")
SET(CPACK_PACKAGE_VERSION "1.0")
SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.3.1-6), libgcc1 (>= 1:3.4.2-12)")
SET(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "mateusz lis mateusz.lis@gmail.com")

SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "color - unix console tool for log syntax coloring")
SET(CPACK_PACKAGE_VENDOR "Mateusz Lis")
SET(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.txt")
SET(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/Copyright.txt")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "CMake ${CMake_VERSION_MAJOR}.${CMake_VERSION_MINOR}")
SET(CPACK_STRIP_FILES "color")
#SET(CPACK_SOURCE_STRIP_FILES "")
SET(CPACK_PACKAGE_EXECUTABLES "color")
INCLUDE(CPack)
