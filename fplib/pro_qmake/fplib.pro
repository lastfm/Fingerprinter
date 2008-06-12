TEMPLATE	= lib
CONFIG	   += warn_on staticlib
CONFIG	   -= qt
QT         -= core gui
TARGET      = fplib

include( ../../../../definitions.pro.inc )

DESTDIR     = $$BUILD_DIR

HEADERS	    = ../include/FingerprintExtractor.h \
	          ../src/CircularArray.h \
        	  ../src/Filter.h \
	          ../src/FloatingAverage.h \
	          ../src/fp_helper_fun.h \
	          ../src/OptFFT.h

SOURCES	    = ../src/FingerprintExtractor.cpp \
	          ../src/Filter.cpp \
	          ../src/OptFFT.cpp

CONFIG(release, release|debug) {
    DEFINES += NDEBUG
}

INCLUDEPATH += ../include \
               ../src \
               ../../libs/fftw/src/api \
               $$ROOT_DIR/res/libsamplerate \
               $$ROOT_DIR/src/libs/libsamplerate/src
               
win32 {
    #QMAKE_LFLAGS += /NODEFAULTLIB
}
               
