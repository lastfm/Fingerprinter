TEMPLATE	= app
CONFIG		= warn_on release
HEADERS	= src/HTTPClient.h \
          src/mbid_mp3.h \
          src/MP3_Source.h \
          src/Sha256.h \
	  src/Sha256File.h

SOURCES	= src/main.cpp \
	  src/HTTPClient.cpp \
	  src/OptFFT.cpp \
          src/Sha256.cpp \
	  src/Sha256File.cpp

DEFINES   = NDEBUG
INCLUDEPATH = src
LIBS  = -L../fplib/ -lfplib
