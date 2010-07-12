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
***************************************************************************/

#ifndef __SHA_FILE_H
#define __SHA_FILE_H

#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>

class Sha256File
{
public:

   static void 
   getHash(const std::string& fileName, unsigned char* pHash);

   static std::string
   toHexString(const unsigned char* pHash, size_t size)
   {
      std::ostringstream oss;
      for (size_t i = 0; i < size; ++i)
         oss << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(pHash[i]);

      return oss.str();
   }

   static int
   hashSize();

private:

   static void 
   getHash( std::ifstream& inFile, unsigned char* pHash );

};

#endif // __SHA_FILE_H
