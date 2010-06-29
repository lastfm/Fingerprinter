In order to compile you will need the following libraries:

libfftw, libtag, libmad, libcurl, libsamplerate

If you are on a debian-like system just do:

apt-get install libtag1c2a libtagc0-dev libcurl4-gnutls-dev libmad0-dev fftw3-dev libsamplerate0-dev
(for curl you can also use libcurl3-dev)

you will also need cmake in order to create the Makefile:

apt-get install cmake

Now just run the command:
cmake .
make

you'll find the binary in ./lastfmfpclient/
or if you want it installed just run

make install

Enjoy! :)
