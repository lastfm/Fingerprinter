In order to compile you will need the following libraries:

libfftw, libtag, libmad, libcurl

If you are on a debian-like system just do:

apt-get install libtag1c2a libtagc0-dev 
apt-get install libcurl4-gnutls-dev
apt-get install libmad0-dev
apt-get install fftw3-dev
apt-get install libsamplerate0-dev

you will also need cmake in order to create the Makefile:

apt-get install cmake

Now just run the command:
cmake .
make

you'll find the binary in ./lastfmfpclient/
or if you want it installed just run

make install

Enjoy! :)
