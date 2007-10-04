/***************************************************************************
 *   Copyright (C) 2005 - 2007 by                                          *
 *      Last.fm Ltd <mir@last.fm>                                          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02111-1307, USA.         *
 *                                                                         *
 ***************************************************************************/

// for fingerprint
#include "../../fplib/include/FingerprintExtractor.h"
#include "MP3_Source.h"
#include "HTTPClient.h"

#include "Sha256File.h" // for SHA 256
#include "mbid_mp3.h"   // for musicbrainz ID

// taglib
#include <fileref.h>
#include <tag.h>
#include <id3v2tag.h>
#include <mpegfile.h>

#include <iostream>
#include <ctime>
#include <map>

using namespace std;

//#ifdef WIN32
//#define SLASH '\\'
//#else
//#define SLASH '/'
//#endif
//

// DO NOT CHANGE THOSE!
const int  FP_PROTOCOL_VERSION = 1;
const char PUBLIC_SERVER_NAME[] = "ws.audioscrobbler.com/fingerprint/";
const char PUBLIC_CLIENT_NAME[] = "FP Beta 1";
const char HTTP_DATA_NAME[] = "fpdata";

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template <typename T>
string toString(const T& val)
{
   // similar to boost::lexical_cast
   ostringstream oss;
   oss << val;
   return oss.str();
}

// -----------------------------------------------------------------------------

string simpleTrim( const string& str )
{
   if ( str.empty() )
      return "";

   // left trim
   string::const_iterator lIt = str.begin();
   for ( ; isspace(*lIt) && lIt != str.end(); ++lIt );
   if ( lIt == str.end() )
      return str;

   string::const_iterator rIt = str.end();
   --rIt;

   for ( ; isspace(*rIt) && rIt != str.begin(); --rIt );
   ++rIt;
   
   return string(lIt, rIt);
}

// -----------------------------------------------------------------------------

void addEntry ( map<string, string>& urlParams, const string& key, const string& val )
{
   if ( urlParams.find(key) != urlParams.end() )
      return; // do not add something that was already there

   urlParams[key] = simpleTrim(val);
}

// -----------------------------------------------------------------------------

void getFileInfo( const string& fileName, map<string, string>& urlParams )
{

   urlParams["fpversion"] = toString(FP_PROTOCOL_VERSION);

   //////////////////////////////////////////////////////////////////////////
   // Musicbrainz ID
   char mbid_ch[MBID_BUFFER_SIZE];
   if ( getMP3_MBID(fileName.c_str(), mbid_ch) != -1 )
      urlParams["mbid"] = string(mbid_ch);

   //////////////////////////////////////////////////////////////////////////
   //// Filename (optional)
   //size_t lastSlash = fileName.find_last_of(SLASH);
   //if ( lastSlash != string::npos )
   //   urlParams["filename"] = fileName.substr(lastSlash+1);
   //else
   //   urlParams["filename"] = fileName;

   ///////////////////////////////////////////////////////////////////////////
   // SHA256  

   const int SHA_SIZE = 32;
   unsigned char sha256[SHA_SIZE]; // 32 bytes
   Sha256File::getHash(fileName, sha256);

   urlParams["sha256"] = Sha256File::toHexString(sha256, SHA_SIZE);

   //////////////////////////////////////////////////////////////////////////
   // ID3

   TagLib::MPEG::File f(fileName.c_str());
   if( f.isValid() && f.tag() ) 
   {
      TagLib::Tag* pTag = f.tag();
      string tmpStr;

      // artist
      addEntry( urlParams, "artist", string(pTag->artist().toCString()) );

      // album
      addEntry( urlParams, "album", string(pTag->album().toCString()) );

      // title
      addEntry( urlParams, "track", string(pTag->title().toCString()) );

      // track num
      if ( pTag->track() > 0 )
         addEntry( urlParams, "tracknum", toString(pTag->track()) );

      if ( pTag->year() > 0 )
         addEntry( urlParams, "year", toString(pTag->year()) );

      // genre
      addEntry( urlParams, "genre", string(pTag->genre().toCString()) );
   }

}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
 
   if ( argc < 2 )
   {
      cout << argv[0] << " (" << PUBLIC_CLIENT_NAME << ")" << endl;
      cout << "A minimal fingerprint client, public release." << endl;
      cout << "Copyright (C) 2007 by Last.fm (MIR)" << endl << endl;
      cout << "Usage: " << argv[0] << " yourMp3File.mp3" << endl;
      exit(0);
   }

   const string mp3FileName = argv[1];

   {
      // check if it exists
      ifstream checkFile(mp3FileName.c_str(), ios::binary);
      if ( !checkFile.is_open() )
      {
         cerr << "ERROR: Cannot find file <" << mp3FileName << ">!" << endl;
         exit(1);
      }

      // checks if it is an mp3
      size_t filelen = mp3FileName.length();
      if ( filelen < 5 || mp3FileName.substr(filelen-4, 4) != ".mp3" )
      {
         cerr << "Sorry, only MP3 files are currently supported. But other formats will follow soon!" << endl;
         exit(1);
      }

   }

   map<string, string> urlParams;
   getFileInfo(mp3FileName, urlParams);

   int duration, samplerate, bitrate, nchannels;
   MP3_Source::getInfo(mp3FileName, duration, samplerate, bitrate, nchannels);

   urlParams["duration"] = toString(duration);
   urlParams["username"] = PUBLIC_CLIENT_NAME;
   urlParams["samplerate"] = toString(samplerate);

   // this will extract the fingerprint
   fingerprint::FingerprintExtractor fextr;
   fextr.initForQuery(samplerate, nchannels);

   // that's for the mp3
   MP3_Source mp3Source;
   const size_t bufSize = 131072;
   short* pPCMBuffer = new short[bufSize];

   try
   {
      mp3Source.init(mp3FileName);
      mp3Source.skipSilence();

      //////////////////////////////////////////////////////////////////////////      
      // that's not really necessary: it's just to speed up things
      mp3Source.skip( static_cast<int>(fextr.getToSkipMs()) );
      fextr.process( 0, static_cast<size_t>(samplerate * nchannels * (fextr.getToSkipMs() / 1000.0)) );
      //////////////////////////////////////////////////////////////////////////      

      for (;;)
      {
         size_t readData = mp3Source.updateBuffer(pPCMBuffer, bufSize);
         if ( readData == 0 )
         {
            cerr << "ERROR: Insufficient input data!" << endl;
            exit(1);
         }

         if ( fextr.process( pPCMBuffer, readData, readData != bufSize ) )
            break;
      }

      pair<const char*, size_t> fpData = fextr.getFingerprint();

      HTTPClient client;
      string c = client.postRawObj( PUBLIC_SERVER_NAME, urlParams, 
                                    fpData.first, fpData.second, 
                                    HTTP_DATA_NAME, true );
      cout << c << flush;
   }
   catch (const std::exception& e)
   {
      cerr << "ERROR: " << e.what() << endl;
      exit(1);
   }

   delete [] pPCMBuffer;
   // bye bye! :)
   return 0;
}

// -----------------------------------------------------------------------------
