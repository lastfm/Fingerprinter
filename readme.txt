last.fm fingerprint
===================

That's the official repository for the last.fm fingerprint library.
The fingerprinting process works in two steps:

1. Get PCM data and pass it to *fplib* which will return byte string to be submitted to the last.fm HTTP fingerprint service. This will return a number (fingerprintID).
2. Query the last.fm API with the fingerprintID and obtain the metadata in xml format.

The lastfmfpclient directory contains an example of application that uses fplib and queries both services.

Building
--------

fplib requires the following libraries
 fftw3-dev
lastfmfpclient requires the following libraries:
 libtag1c2a libtagc0-dev libcurl4-gnutls-dev libmad0-dev libsamplerate0-dev

(see also readme-unix.txt)

To compile both fplib and lastfmfpclient just run (you'll need cmake):
   $ cmake .
   $ make
   $ sudo make install

Running
-------

To use lastfmfpclient just pass any mp3 as argument

   $ lastfmfpclient mysterious.mp3

Using fplib
===========

You can freely use fplib in your application without any restriciton (see license.txt). Accessing the last.fm API services is restricted to our terms of services (see http://www.last.fm/api/tos). Basically you can do pretty much whatever you want as long as it's not for profit. 

If you plan to use fplib in your application, there are a few VERY important things to keep in mind:

important things to keep in mind
--------------------------------

a. fplib accepts standard PCM data, which means that if your track is encoded with some format (mp3, ogg, wma, etc..) you need to decode it first. 
b. IMPORTANT: The library *REQUIRES* the data to be from the beginning of the file. 
c. You don't have to decode the whole thing as the library can get as many bytes as you want to pass it. If it needs more it will return false. Basically the whole processing steps can be summarized by this loop:

      for (;;)
      {
         // read some data from the file
         size_t sz = read_some_source( pPCMBuffer );
         if ( sz == 0 )
         {
            cerr << "ERROR: Insufficient input data!" << endl;
            exit(1); // goodbye!
         }

         // Process to create the fingerprint. If process returns true
         // it means he's happy with what he has.
         if ( fextr.process( pPCMBuffer, sz /*, bool: set it to true if it's EOF */ ) )
            break;
      }

      // get the fingerprint data
      pair<const char*, size_t> fpData = fextr.getFingerprint();
 
d. fpData is what you'll need to send to the HTTP fingerprint service along with (also VERY important) the length of the track in seconds.
e. Other stuff to send is nice to have (artist and track names, genre, username, etc..) but not mandatory (but if you abuse us, we'll get you)

The protocol used to talk to the HTTP fingerprint service is just a POST with the fingerprint binary as object, and the other things (length, metadata, etc..) as parameters. See lastfmfpclient for an example.

Using the metadata API
======================

Once you have obtained the fingerprintID from the HTTP fingerprint service (see above) you have to talk to the metadata API to get the XML with the description of the track.

To do so, you have to obtain an API key (http://www.last.fm/api/account) then just query the service with the following parameters:

http://ws.audioscrobbler.com/2.0/?method=track.getfingerprintmetadata&fingerprintid=THE_FINGERPRINT_ID&api_key=YOUR_API_KEY
