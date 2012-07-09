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

   # Library paths paths
   LINK_DIRECTORIES(${PROJECT_SOURCE_DIR}/lib)

   # Release build
   SET(CMAKE_BUILD_TYPE release)
   ADD_DEFINITIONS(-O2 -DNDEBUG)

   # Use additional paths when building on a Mac
   IF(APPLE)
      INCLUDE_DIRECTORIES(/opt/local/include/)
      INCLUDE_DIRECTORIES(/opt/local/include/taglib)
      LINK_DIRECTORIES(/opt/local/lib)
   ENDIF(APPLE)


   # Create uninstall target
   FILE(WRITE cmake_uninstall.cmake.in
      "IF(COMMAND cmake_policy)\n"
      "   IF(POLICY CMP0007)\n"
      "      CMAKE_POLICY(SET CMP0007 OLD)\n"
      "   ENDIF(POLICY CMP0007)\n"
      "ENDIF(COMMAND cmake_policy)\n"
      "\n"
      "IF (NOT EXISTS \"\@CMAKE_CURRENT_BINARY_DIR\@/install_manifest.txt\")\n"
      "    MESSAGE(FATAL_ERROR \"Cannot find install manifest: @CMAKE_CURRENT_BINARY_DIR\@/install_manifest.txt\")\n"
      "ENDIF(NOT EXISTS \"\@CMAKE_CURRENT_BINARY_DIR\@/install_manifest.txt\")\n"
      "\n"
      "FILE(READ \"\@CMAKE_CURRENT_BINARY_DIR\@/install_manifest.txt\" files)\n"
      "STRING(REGEX REPLACE \"\\n\" \";\" files \"\${files}\")\n"
      "LIST(REVERSE files)\n"
      "\n"
      "FOREACH (file \${files})\n"
      "    MESSAGE(STATUS \"Uninstalling \$ENV{DESTDIR}\${file}\")\n"
      "    if (EXISTS \"\$ENV{DESTDIR}\${file}\")\n"
      "        EXECUTE_PROCESS(\n"
      "            COMMAND \@CMAKE_COMMAND\@ -E remove \"\$ENV{DESTDIR}\${file}\"\n"
      "            OUTPUT_VARIABLE rm_out\n"
      "            RESULT_VARIABLE rm_retval\n"
      "        )\n"
      "        IF(NOT \${rm_retval} EQUAL 0)\n"
      "            MESSAGE(FATAL_ERROR \"Problem when removing \$ENV{DESTDIR}\${file}\")\n"
      "        ENDIF (NOT \${rm_retval} EQUAL 0)\n"
      "    ELSE (EXISTS \"\$ENV{DESTDIR}\${file}\")\n"
      "        MESSAGE(STATUS \"File \$ENV{DESTDIR}\${file} does not exist.\")\n"
      "    ENDIF (EXISTS \"\$ENV{DESTDIR}\${file}\")\n"
      "ENDFOREACH(file)\n"
   )

   CONFIGURE_FILE(
      "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
      "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
      IMMEDIATE @ONLY
      )

   ADD_CUSTOM_TARGET(uninstall
      COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)

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
