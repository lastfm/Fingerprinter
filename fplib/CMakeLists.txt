PROJECT(fplib)

CMAKE_MINIMUM_REQUIRED(VERSION 2.4)

# uncomment if you wanna see some verbose action
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# binaries get installed here
SET(CMAKE_INSTALL_PREFIX "/usr/")

#add definitions, compiler switches, etc.
ADD_DEFINITIONS(-Wall -O2 -DNDEBUG -g `getconf LFS_CFLAGS`)

INCLUDE_DIRECTORIES(include src)

ADD_LIBRARY(fplib STATIC
            src/Filter
            src/FingerprintExtractor
            src/OptFFT
            )
            
INSTALL(TARGETS fplib ARCHIVE DESTINATION lib)
INSTALL(DIRECTORY include/fplib DESTINATION include PATTERN ".svn" EXCLUDE)
