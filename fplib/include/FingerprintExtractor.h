/***************************************************************************
* This file is part of last.fm fingerprint app                             *
*  Last.fm Ltd <mir@last.fm>                                               *
*                                                                          *
* This library is free software; you can redistribute it and/or            *
* modify it under the terms of the GNU Lesser General Public               *
* License as published by the Free Software Foundation; either             *
* version 2.1 of the License, or (at your option) any later version.       *
*                                                                          *
* This library is distributed in the hope that it will be useful,          *
* but WITHOUT ANY WARRANTY; without even the implied warranty of           *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU        *
* Lesser General Public License for more details.                          *
*                                                                          *
* You should have received a copy of the GNU Lesser General Public         *
* License along with this library; if not, write to the Free Software      *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 *
* USA                                                                      *
*                                                                          *
* Part of this code is based on the work of Y. Ke, D. Hoiem, and           *
* R. Sukthankar - "Computer Vision for Music Identification",              *
* in Proceedings of Computer Vision and Pattern Recognition, 2005.         *
* See also http://www.cs.cmu.edu/~yke/musicretrieval/                      *
***************************************************************************/

#ifndef __FINGERPRINT_EXTRACTOR_H
#define __FINGERPRINT_EXTRACTOR_H

#include <utility> // for pair
#include <cstddef> // for size_t

namespace fingerprint {

// -----------------------------------------------------------------------------

class PimplData;

class FingerprintExtractor
{
public:

   FingerprintExtractor(); // ctor
   ~FingerprintExtractor(); // dtor

   // duration (in seconds!) is optional, but if you want to submit tracks <34 secs
   // it must be provided. 
   void initForQuery(int freq, int nchannels, int duration = -1);
   void initForFullSubmit(int freq, int nchannels);

   // return false if it needs more data, otherwise true
   // IMPORTANT: num_samples specify the size of the *short* array pPCM, that is
   //            the number of samples that are in the buffer. This includes
   //            the stereo samples, i.e.
   //            [L][R][L][R][L][R][L][R] would be num_samples=8
   bool process(const short* pPCM, size_t num_samples, bool end_of_stream = false);

   // returns pair<NULL, 0> if the data is not ready
   std::pair<const char*, size_t> getFingerprint();

   //////////////////////////////////////////////////////////////////////////

   // The FingerprintExtractor assumes that the file start from the beginning
   // but since the first SkipMs are ignored, it's possible to feed it with NULL.
   // In order to know how much must be skipped (in milliseconds) call this function.
   // Remark: this is only for "advanced" users!
   size_t getToSkipMs();

   // Return the minimum duration of the file (in ms)
   // Any file with a length smaller than this value will be discarded
   static size_t getMinimumDurationMs();

   // return the version of the fingerprint
   static size_t getVersion();

private:

   PimplData* m_pPimplData;
};

// -----------------------------------------------------------------------------

} // end of namespace fingerprint

#endif // __FINGERPRINT_EXTRACTOR_H
