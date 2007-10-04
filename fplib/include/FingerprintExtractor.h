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
 *   Part of this code is based on the work of Y. Ke, D. Hoiem, and        *
 *   R. Sukthankar - "Computer Vision for Music Identification",           *
 *   in Proceedings of Computer Vision and Pattern Recognition, 2005.      *
 *   See also http://www.cs.cmu.edu/~yke/musicretrieval/                   *
 ***************************************************************************/

#ifndef __FINGERPRINT_EXTRACTOR_H
#define __FINGERPRINT_EXTRACTOR_H

#include <utility> // for pair

namespace fingerprint {

// -----------------------------------------------------------------------------

class PimplData;

class FingerprintExtractor
{
public:

   FingerprintExtractor(); // ctor
   ~FingerprintExtractor(); // dtor

   void initForQuery(int freq, int nchannels);
   void initForFullSubmit(int freq, int nchannels);

   // return false if it needs more data, otherwise true
   // input MUST BE PCM data (16bits).
   // size % 16 == 0 gives the best performance
   // IMPORTANT: Assuming the data starts at 7.5 seconds from the beginning!
   //            (10 secs - half normalization window)
   // when true is returned, just call getFingerprint to get the fingerprint
   bool process(const short* pPCM, size_t size, bool end_of_stream = false);

   // returns pair<NULL, 0> if the data is not ready
   std::pair<const char*, size_t> getFingerprint();

   //////////////////////////////////////////////////////////////////////////

   // The FingerprintExtractor assumes that the file start from the beginning
   // but since the first SkipMs are ignored, it's possible to feed it with NULL.
   // In order to know how much must be skipped (in milliseconds) call this function.
   // Remark: this is only for "advanced" users!
   size_t getToSkipMs();

private:

   PimplData* m_pPimplData;

};

// -----------------------------------------------------------------------------

} // end of namespace fingerprint

#endif // __FINGERPRINT_EXTRACTOR_H
