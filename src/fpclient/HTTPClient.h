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

#ifndef LASTFM_LIBFPLIB_HTTP_CLIENT_H__
#define LASTFM_LIBFPLIB_HTTP_CLIENT_H__

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

   std::string get(const std::string& url);

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

#endif

// -----------------------------------------------------------------------------
