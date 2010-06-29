CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

PROJECT(lastfmfpclient)

#if youthe full compiler output, uncomment the following line
# SET(CMAKE_VERBOSE_MAKEFILE ON)

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2 -g -DNDEBUG)

INCLUDE_DIRECTORIES(src /usr/local/include/taglib /usr/include/taglib)
LINK_DIRECTORIES(../fplib)

IF(APPLE)
INCLUDE_DIRECTORIES(/opt/local/include/)
INCLUDE_DIRECTORIES(/opt/local/include/taglib)
LINK_DIRECTORIES(/opt/local/lib)
ENDIF(APPLE)


#list all source files here
ADD_EXECUTABLE( lastfmfpclient
                  src/main.cpp
                  src/HTTPClient.cpp
                  src/MP3_Source.cpp
                  src/Sha256.cpp
                  src/Sha256File.cpp )

TARGET_LINK_LIBRARIES(lastfmfpclient lastfmfp fftw3f mad tag curl samplerate)

INSTALL(TARGETS lastfmfpclient
        RUNTIME DESTINATION bin
        COMPONENT lastfmfpclient)

SET(CPACK_GENERATOR "DEB")
SET(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
SET(CPACK_SET_DESTDIR TRUE)
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Norman Casagrande <norman@last.fm>")
SET(CPACK_PACKAGE_VERSION_MAJOR "1")
SET(CPACK_PACKAGE_VERSION_MINOR "6")
SET(CPACK_PACKAGE_VERSION_PATCH "0")

SET( CPACK_DEBIAN_PACKAGE_DEPENDS "liblastfmfp,libtag1c2a,libtagc0,libcurl3,libmad0,libfftw3-3,libsamplerate0" )

INCLUDE(CPack)
