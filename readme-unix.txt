In order to compile you will need the following libraries:

libfftw, libtag, libmad, libcurl

If you are on a debian-like system just do:

apt-get install libtag1c2a libtagc0-dev 
apt-get install libcurl3-dev
apt-get install libmad0-dev
apt-get install fftw3-dev
apt-get install libsamplerate0-dev

you will also need tmake in order to create the Makefile:

apt-get install tmake.

Now just run the command:
tmake -o Makefile buildall.pro && make || echo "MASSIVE FAIL"

you'll find the binary in ./lastfmfpclient/

Enjoy! :)
