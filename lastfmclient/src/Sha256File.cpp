/***************************************************************************
 *   Copyright (C) 2005 - 2007 by                                          *
 *      Last.fm Ltd <mir@last.fm>                                       *
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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02111-1307, USA.          *
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
