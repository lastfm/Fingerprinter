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


#ifndef __PCM_SOURCE_H
#define __PCM_SOURCE_H

#include <vector>

using namespace std;

// -----------------------------------------------------------------------------

class PCM_Source
{
public:

//#ifdef WIN32
//    virtual void  init(const wstring& fileName) = 0;
//#else
    virtual void  init(const string& fileName) = 0;
//#endif

    virtual void  release() = 0;
   // skip n seconds
   virtual void skip(const int mSecs) = 0;

   // try to refill the buffer with the next chunk of the audio stream
   // Returns:  the number of used/valid elements in the buffer
   // Note: If toRead == 0 then it fill the whole buffer
   virtual int    updateBuffer(signed short* pBuffer, size_t bufferSize) = 0;

};

// -----------------------------------------------------------------------------

#endif
