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

TARGET_LINK_LIBRARIES(lastfmfpclient fplib fftw3f mad tag curl samplerate)

INSTALL(TARGETS lastfmfpclient
        RUNTIME DESTINATION bin
        COMPONENT lastfmfpclient)
