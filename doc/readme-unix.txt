In order to compile you will need the following libraries:

libfftw, libtag, libmad, libcurl, libsamplerate

If you are on a debian-like system just do:

apt-get install libtag1c2a libtagc0-dev libmad0-dev fftw3-dev libsamplerate0-dev libcurl3 libcurl3-dev

you will also need cmake in order to create the Makefile:

apt-get install cmake

Now just run the command:
cmake .
make

you'll find the binary in ./lastfm-fpclient/

To create a debian package just run the following command:

dpkg-buildpackage

To create API documentation just type:

doxygen

NB. You'll need to install doxygen and graphviz dot

apt-get install doxygen graphviz

Enjoy! :)
