CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Last.fm Fingerprint Library and Sample Client
PROJECT(lastfm_fplib)

# override default build settings using cmake config file
INCLUDE(config.cmake OPTIONAL
   RESULT_VARIABLE config_file
   )

# Add this libraries include path
INCLUDE_DIRECTORIES(include)

# if no config file was found using these settings
IF(config_file STREQUAL NOTFOUND)

   # Setup output directories
   SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
   SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
   SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)

   # Set install path prefix
   SET(CMAKE_INSTALL_PREFIX /usr/local)

   # Release build
   SET(CMAKE_BUILD_TYPE release)
   ADD_DEFINITIONS(-O2 -DNDEBUG)

   # Use additional paths when building on a Mac
   IF(APPLE)
      INCLUDE_DIRECTORIES(/opt/local/include/)
      INCLUDE_DIRECTORIES(/opt/local/include/taglib)
      LINK_DIRECTORIES(/opt/local/lib)
   ENDIF(APPLE)

ENDIF()

# Required for taglib
INCLUDE_DIRECTORIES(/usr/include/taglib)

##############################################################################
# Build library

# The source files
SET(lfmfp_sources
   src/fplib/Filter
   src/fplib/FingerprintExtractor
   src/fplib/OptFFT
   )
# Define static library
ADD_LIBRARY(fplib_static STATIC
   ${lfmfp_sources}
   )

# Define dynamic library
ADD_LIBRARY(fplib_shared SHARED
   ${lfmfp_sources}
   )

# The static library is called 'libfplib.a'
SET_TARGET_PROPERTIES(fplib_static
   PROPERTIES OUTPUT_NAME fplib
   )

# The shared library is called 'libfplib.so'
SET_TARGET_PROPERTIES(fplib_shared
   PROPERTIES OUTPUT_NAME fplib
   )

# Install static library
INSTALL(TARGETS fplib_static
   ARCHIVE DESTINATION lib
   )

# Install dynamic library
INSTALL(TARGETS fplib_shared
   LIBRARY DESTINATION lib
   )

# Install header files
INSTALL(DIRECTORY include/fplib
   DESTINATION include
   )
##############################################################################
# Build sample client

# Define the executable
ADD_EXECUTABLE(lastfm-fpclient
   src/fpclient/main.cpp
   src/fpclient/HTTPClient.cpp
   src/fpclient/MP3_Source.cpp
   src/fpclient/Sha256.cpp
   src/fpclient/Sha256File.cpp
   )

# Link the executable
TARGET_LINK_LIBRARIES(lastfm-fpclient
   fplib
   fftw3f
   mad
   tag
   curl
   samplerate
   )

# Install the executable
INSTALL(TARGETS lastfm-fpclient
   RUNTIME DESTINATION bin
   COMPONENT lastfmfpclient
   )

##############################################################################
