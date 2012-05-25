/***************************************************************************
* This file is part of last.fm fingerprint app                             *
*  Last.fm Ltd <mir@last.fm>                                               *
*                                                                          *
* The last.fm fingerprint app is free software: you can redistribute it    *
* and/or modify it under the terms of the GNU General Public License as    *
* published by the Free Software Foundation, either version 3 of the       *
* License, or (at your option) any later version.                          *
*                                                                          *
* The last.fm fingerprint app and library are distributed in the hope that *
* it will be useful, but WITHOUT ANY WARRANTY; without even the implied    *
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See    *
* the GNU General Public License for more details.                         *
*                                                                          *
* You should have received a copy of the GNU General Public License        *
* along with The last.fm fingerprint.                                      *
* If not, see <http://www.gnu.org/licenses/>.                              *
*                                                                          *
* Part of this code is based on the work of Y. Ke, D. Hoiem, and           *
* R. Sukthankar - "Computer Vision for Music Identification",              *
* in Proceedings of Computer Vision and Pattern Recognition, 2005.         *
* See also http://www.cs.cmu.edu/~yke/musicretrieval/                      *
***************************************************************************/

#ifndef LASTFM_LIBFPLIB_MP3_SOURCE_H__
#define LASTFM_LIBFPLIB_MP3_SOURCE_H__

#include <string>
#include <vector>
#include <fstream>

#include <mad.h>

using namespace std;

// ----------------------------------------------------------------------- ------

class MP3_Source
{
public:

   // ctor
   MP3_Source();
   virtual ~MP3_Source();

   static void getInfo(const string& fileName, int& lengthSecs, int& samplerate, int& bitrate, int& nchannels);
   virtual void  init(const string& fileName);
   virtual void  release();

   // return a chunk of PCM data from the mp3
   virtual int updateBuffer(signed short* pBuffer, size_t bufferSize);

   virtual void skip(const int mSecs);
   virtual void skipSilence(double silenceThreshold = 0.0001);

   bool  eof() const { return m_inputFile.eof() && m_pcmpos == 0; }

private:

   static bool fetchData( ifstream& mp3File,
                          unsigned char* pMP3_Buffer,
                          const int MP3_BufferSize,
                          mad_stream& madStream );

   static bool isRecoverable(const mad_error& error, bool log = false);

   static string MadErrorString(const mad_error& error);

   struct mad_stream	m_mad_stream;
   struct mad_frame	m_mad_frame;
   mad_timer_t			m_mad_timer;
   struct mad_synth	m_mad_synth;

   string            m_fileName;
   ifstream          m_inputFile;

   unsigned char*    m_pMP3_Buffer;
   static const int  m_MP3_BufferSize = (5*8192);

   size_t            m_pcmpos;
};

// ---------------------------------------------------------------------

// -----------------------------------------------------------------------------

#endif
