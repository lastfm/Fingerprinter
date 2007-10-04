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
 *   51 Franklin Steet, Fifth Floor, Boston, MA  02111-1307, USA.          *
 ***************************************************************************/


#ifndef __HTTP_CLIENT_H
#define __HTTP_CLIENT_H

#include <curl/curl.h>

#include <string>
#include <vector>
#include <map>

// -----------------------------------------------------------------------------

class HTTPClient
{
public:

   HTTPClient();
   ~HTTPClient();

   bool setCookie(const std::string& cookieFileName);

   void fetchWebPage(const std::string& url, 
                     std::string& thePage);

   // i.e.
   // map<string, string> m;
   // m["user"]="me";
   // m["age"]="18";
   // PICTURE pic;
   // postRawObj("www.mytest.com", m, &pic, sizeof(pic) );
   std::string postRawObj( const std::string& url, const std::map<std::string, std::string>& urlParams, 
                           const char* pData, size_t dataSize, const std::string& dataName = "",
                           bool encode = false );

   std::string postRawObj( const std::string& url,
                           const char* pData, size_t dataSize, const std::string& dataName = "",
                           bool encode = false );

private:

   static size_t httpFetch(void* ptr, size_t size, size_t nmemb, void *pData);

   CURL*                m_pCurlHandle;
   std::vector<char>    m_inBuffer;
   char*                m_pOutIt;

};

#endif // __HTTP_CLIENT_H

// -----------------------------------------------------------------------------
