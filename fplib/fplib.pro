TEMPLATE	= lib
CONFIG		= warn_on staticlib
HEADERS	= include/FingerprintExtractor.h \
	  src/CircularArray.h \
	  src/Filter.h \
	  src/FloatingAverage.h \
	  src/fp_helper_fun.h \
	  src/OptFFT.h

SOURCES	= src/FingerprintExtractor.cpp \
	  src/Filter.cpp \
	  src/OptFFT.cpp

DEFINES   += NDEBUG

INCLUDEPATH = include src
