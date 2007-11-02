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

#include "MP3_Source.h" // to decode mp3s
#include "HTTPClient.h" // for connection

#include "Sha256File.h" // for SHA 256
#include "mbid_mp3.h"   // for musicbrainz ID

// taglib
#include <fileref.h>
#include <tag.h>
#include <id3v2tag.h>
#include <mpegfile.h>

// stl
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using namespace std;

// hacky!
#ifdef WIN32
#define SLASH '\\'
#else
#define SLASH '/'
#endif

// DO NOT CHANGE THOSE!
const char FP_SERVER_NAME[]       = "ws.audioscrobbler.com/fingerprint/query/";
const char METADATA_SERVER_NAME[] = "ws.audioscrobbler.com/fingerprint/fp.php";
const char PUBLIC_CLIENT_NAME[]   = "FP Beta 1.31";
const char HTTP_POST_DATA_NAME[]  = "fpdata";

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

// just turn it into a string. Similar to boost::lexical_cast
template <typename T>
std::string toString(const T& val)
{
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
   if ( key.empty() || val.empty() )
      return;
   if ( urlParams.find(key) != urlParams.end() )
      return; // do not add something that was already there

   urlParams[key] = simpleTrim(val);
}

// -----------------------------------------------------------------------------

// gather some data from the (mp3) file
void getFileInfo( const string& fileName, map<string, string>& urlParams )
{

   //////////////////////////////////////////////////////////////////////////
   // Musicbrainz ID
   char mbid_ch[MBID_BUFFER_SIZE];
   if ( getMP3_MBID(fileName.c_str(), mbid_ch) != -1 )
      urlParams["mbid"] = string(mbid_ch);

   //////////////////////////////////////////////////////////////////////////
   // Filename (optional)
   size_t lastSlash = fileName.find_last_of(SLASH);
   if ( lastSlash != string::npos )
      urlParams["filename"] = fileName.substr(lastSlash+1);
   else
      urlParams["filename"] = fileName;

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

      // artist
      addEntry( urlParams, "artist", string(pTag->artist().toCString()) );

      // album
      addEntry( urlParams, "album", string(pTag->album().toCString()) );

      // title
      addEntry( urlParams, "track", string(pTag->title().toCString()) );

      // track num
      if ( pTag->track() > 0 )
         addEntry( urlParams, "tracknum", toString(pTag->track()) );

      // year
      if ( pTag->year() > 0 )
         addEntry( urlParams, "year", toString(pTag->year()) );

      // genre
      addEntry( urlParams, "genre", string(pTag->genre().toCString()) );
   }

}

// -----------------------------------------------------------------------------

string fetchMetadata(int fpid, HTTPClient& client)
{
   string c;
   // it's in there! let's get the metadata
   ostringstream oss; 
   oss << METADATA_SERVER_NAME << "?fid=" << fpid;
   c = client.get(oss.str());

   if ( c.empty() )
      cout << "No metadata found (yet!).. :(" << endl;

   return c;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

int main(int argc, char* argv[])
{
   if ( argc < 2 )
   {
      string fileName = string(argv[0]);
      size_t lastSlash = fileName.find_last_of(SLASH);
      if ( lastSlash != string::npos )
         fileName = fileName.substr(lastSlash+1);      

      cout << fileName << " (" << PUBLIC_CLIENT_NAME << ")" << endl;
      cout << "A minimal fingerprint client, public release." << endl;
      cout << "Copyright (C) 2007 by Last.fm (MIR) - Build: " << __DATE__ << " (" << __TIME__ << ")" << endl << endl;
      cout << "Usage: " << endl;
      cout << fileName << " yourMp3File.mp3" << endl;
      cout << "or" << endl;
      cout << fileName << " -nometadata yourMp3File.mp3" << endl;
      cout << "(if no metadata is wanted)" << endl;
      exit(0);
   }

   string mp3FileName;
   bool wantMetadata = true;
   
   if ( argc == 2 )
      mp3FileName = argv[1];
   else if ( argc == 3 )
   {
      if ( string(argv[1]) == "-nometadata" )
      {
         wantMetadata = false;
         mp3FileName = argv[2];
      }
      else
      {
         cerr << "ERROR: Invalid option " << argv[1] << endl;
         exit(1);
      }
   }
   else
   {
      cerr << "ERROR: Invalid arguments" << endl;
      exit(1);
   }

   //////////////////////////////////////////////////////////////////////////
   
   {
      // check if it exists
      ifstream checkFile(mp3FileName.c_str(), ios::binary);
      if ( !checkFile.is_open() )
      {
         cerr << "ERROR: Cannot find file <" << mp3FileName << ">!" << endl;
         exit(1);
      }

      // checks if it is an mp3 (very un-elegant)
      size_t filelen = mp3FileName.length();
      if ( filelen < 5 || mp3FileName.substr(filelen-4, 4) != ".mp3" )
      {
         cerr << "Sorry, only MP3 files are currently supported. But other formats will follow soon!" << endl;
         exit(1);
      }

   }

   // this map holds the parameters that will be put into the URL
   map<std::string, std::string> urlParams;

   getFileInfo(mp3FileName, urlParams);

   int duration, samplerate, bitrate, nchannels;
   MP3_Source::getInfo(mp3FileName, duration, samplerate, bitrate, nchannels);

   if ( static_cast<size_t>(duration * 1000) < fingerprint::FingerprintExtractor::getMinimumDurationMs() )
   {
      cerr << "ERROR: Song duration is " << duration 
           << "s! Minimum required is: " 
           << static_cast<int>( fingerprint::FingerprintExtractor::getMinimumDurationMs() / 1000 ) 
           << "s" << endl;
      exit(1);
   }

   urlParams["duration"]   = toString(duration); // this is absolutely mandatory
   urlParams["username"]   = PUBLIC_CLIENT_NAME; // replace with username if possible
   urlParams["samplerate"] = toString(samplerate);

   // This will extract the fingerprint
   // IMPORTANT: FingerprintExtractor assumes the data starts from the beginning of the file!
   fingerprint::FingerprintExtractor fextr;
   fextr.initForQuery(samplerate, nchannels, duration); // initialize for query

   size_t version = fextr.getVersion();
   // wow, that's odd.. If I god directly with getVersion I get a strange warning with VS2005.. :P
   urlParams["fpversion"]  = toString( version ); 

   // that's for the mp3
   MP3_Source mp3Source;
   // the buffer can be any size, but FingerprintExtractor is happier (read: faster) with 2^x
   const size_t PCMBufSize = 131072; 
   short* pPCMBuffer = new short[PCMBufSize];

   try
   {
      mp3Source.init(mp3FileName);
      mp3Source.skipSilence();

      //////////////////////////////////////////////////////////////////////////      
      // that's not mandatory: it's just to speed up things. 
      // IMPORTANT: DO NOT DO IT WHEN FingerprintExtractor HAS BEEN SET TO initForFullSubmit !!!
      mp3Source.skip( static_cast<int>(fextr.getToSkipMs()) );
      // send a null pointer, since that's data it's ignored anyway
      fextr.process( 0, static_cast<size_t>(samplerate * nchannels * (fextr.getToSkipMs() / 1000.0)) );
      //////////////////////////////////////////////////////////////////////////      

      for (;;)
      {
         // read some data from the mp3
         size_t readData = mp3Source.updateBuffer(pPCMBuffer, PCMBufSize);
         if ( readData == 0 )
         {
            cerr << "ERROR: Insufficient input data!" << endl;
            exit(1);
         }

         // Process to create the fingerprint. If process returns true
         // it means he's happy with what he has.
         if ( fextr.process( pPCMBuffer, readData, mp3Source.eof() ) )
            break;
      }

      // get the fingerprint data
      pair<const char*, size_t> fpData = fextr.getFingerprint();

      // send the fingerprint data, and get the fingerprint ID
      HTTPClient client;
      string c = client.postRawObj( FP_SERVER_NAME, urlParams, 
                                    fpData.first, fpData.second, 
                                    HTTP_POST_DATA_NAME, false );
      int fpid;
      istringstream iss(c);
      iss >> fpid;

      if ( !wantMetadata && iss.fail() )
         c = "-1 FAIL"; // let's keep it parseable: we had an error!
      else if ( wantMetadata && !iss.fail() )
      {
         // if there was no error and it wants metadata
         string state;
         iss >> state;
         if ( state == "FOUND" )
            c = fetchMetadata(fpid, client);
         else if ( state == "NEW" )
         {
            cout << "Was not found! Now added, thanks! :)" << endl;
            c.clear();
         }
      }

      // if iss.fail() this will display the error, otherwise it will display
      // metadata or the id

      cout << c;

   }
   catch (const std::exception& e)
   {
      cerr << "ERROR: " << e.what() << endl;
      exit(1);
   }

   // clean up!
   delete [] pPCMBuffer;

   // bye bye and thanks for all the fish!
   return 0;
}

// -----------------------------------------------------------------------------
