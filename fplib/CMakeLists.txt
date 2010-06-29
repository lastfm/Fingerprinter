PROJECT(liblastfmfp)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# uncomment if you wanna see some verbose action
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# binaries get installed here
SET(CMAKE_INSTALL_PREFIX "/usr/")

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2 -DNDEBUG -g `getconf LFS_CFLAGS`)

INCLUDE_DIRECTORIES(include src)

IF(APPLE)
INCLUDE_DIRECTORIES(/opt/local/include/)
ENDIF(APPLE)

ADD_LIBRARY(lastfmfp STATIC
            src/Filter
            src/FingerprintExtractor
            src/OptFFT
            )
            
INSTALL(TARGETS lastfmfp ARCHIVE DESTINATION lib)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_SET_DESTDIR TRUE)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "norman")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "6")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

INCLUDE(CPack)
