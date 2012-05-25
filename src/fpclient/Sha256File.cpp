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

#include "Sha256File.h"
#include "Sha256.h"

#include <fstream>

uint8_t pBuffer[SHA_BUFFER_SIZE+7];

using namespace std;

// -----------------------------------------------------------------------------

void 
Sha256File::getHash(const string& fileName, unsigned char* pHash)
{
   ifstream inFile(fileName.c_str(), ios::binary);
   getHash(inFile, pHash);
}

// -----------------------------------------------------------------------------

void Sha256File::getHash( std::ifstream& inFile, unsigned char* pHash )
{
   SHA256Context sha256;
   SHA256Init(&sha256);

   uint8_t* pMovableBuffer = pBuffer;

   // Ensure it is on a 64-bit boundary. 
   INTPTR offs;
   if ((offs = reinterpret_cast<INTPTR>(pBuffer) & 7L))
      pMovableBuffer += 8 - offs;

   unsigned int len;

   for (;;)
   {
      inFile.read( reinterpret_cast<char*>(pMovableBuffer), SHA_BUFFER_SIZE );
      len = inFile.gcount();

      if ( len == 0)
         break;

      SHA256Update (&sha256, pMovableBuffer, len);
   }

   SHA256Final (&sha256, pHash);
}

// -----------------------------------------------------------------------------

int 
Sha256File::hashSize()
{
   return SHA256_HASH_SIZE;
}

// -----------------------------------------------------------------------------
