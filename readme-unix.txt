In order to compile you will need the following libraries:

libfftw, libtag, libmad, libcurl

If you are on a debian-like system just do:

apt-get install libtag1c2a
apt-get install libcurl4-gnutls-dev (or libcurl3)
apt-get install libmad0
apt-get install fftw3

you will also need tmake in order to create the Makefile:

apt-get install tmake.

Now just run the command:
tmake -o Makefile buildall.pro; make

you'll find the binary in ./lastfmfpclient/

Enjoy! :)
