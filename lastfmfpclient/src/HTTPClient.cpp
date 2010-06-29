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

#include "HTTPClient.h"

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cstring>
using namespace std;

// -----------------------------------------------------------------------------

HTTPClient::HTTPClient()
{
   // in windows, this will init the winsock stuff
   curl_global_init(CURL_GLOBAL_ALL);
   m_pCurlHandle = curl_easy_init();

   if ( !m_pCurlHandle )
   {
      cerr << "ERROR: Cannot initialize the curl handler!" << endl;
      exit(1);
   }

   // send all data to this function
   curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEFUNCTION, httpFetch);
   // we pass our buffer vector to the callback function
   curl_easy_setopt(m_pCurlHandle, CURLOPT_WRITEDATA, (void *)&m_inBuffer);

   // some servers don't like requests that are made without a user-agent
   // field, so we provide one
   curl_easy_setopt(m_pCurlHandle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
}

// -----------------------------------------------------------------------------

HTTPClient::~HTTPClient()
{
   curl_easy_cleanup(m_pCurlHandle);
}

// -----------------------------------------------------------------------------

bool HTTPClient::setCookie( const string& cookieFileName )
{
   if (curl_easy_setopt(m_pCurlHandle, CURLOPT_COOKIEFILE, cookieFileName.c_str()) != 0)
      return false;

   return true;
}

// -----------------------------------------------------------------------------

string HTTPClient::postRawObj( const std::string& url, 
                               const char* pData, size_t dataSize,
                               const string& dataName,
                               bool encode )
{
   map<string, string> emptyUrlParams;
   return postRawObj(url, emptyUrlParams, pData, dataSize, dataName, encode);
}

// -----------------------------------------------------------------------------

string HTTPClient::postRawObj( const std::string& url, 
                               const map<string, string>& urlParams, 
                               const char* pData, size_t dataSize,
                               const string& dataName, bool encode )
{
   m_inBuffer.clear();
   string dName;
   if ( dataName.empty() )
      dName = "bindata";
   else
      dName = dataName;

   string newUrl = url;

   struct curl_slist*    pHeaderlist = NULL;
   struct curl_httppost* pFormpost = NULL;
   struct curl_httppost* pLast = NULL;

   char* pDataEnc = NULL;

   if ( encode )
      pDataEnc = curl_easy_escape(m_pCurlHandle, pData, static_cast<int>(dataSize) );
   else
   {
      curl_formadd( &pFormpost, &pLast, 
                    CURLFORM_PTRNAME, dName.c_str(),
                    CURLFORM_PTRCONTENTS, pData, 
                    CURLFORM_CONTENTSLENGTH, dataSize,
                    CURLFORM_END);
   }

   if ( !urlParams.empty() )
   {
      ostringstream oss;
      map<string, string>::const_iterator mIt = urlParams.begin();

      char* pEntryName = curl_easy_escape(m_pCurlHandle, mIt->first.c_str(), static_cast<int>(mIt->first.size()));
      char* pEntryVal = curl_easy_escape(m_pCurlHandle, mIt->second.c_str(), static_cast<int>(mIt->second.size()));
      oss << url << '?' << pEntryName
                 << '=' << pEntryVal;

      curl_free(pEntryName);
      curl_free(pEntryVal);
          
      ++mIt;

      for ( ; mIt != urlParams.end(); ++mIt )
      {
         pEntryName = curl_easy_escape(m_pCurlHandle, mIt->first.c_str(), static_cast<int>(mIt->first.size()));
         pEntryVal = curl_easy_escape(m_pCurlHandle, mIt->second.c_str(), static_cast<int>(mIt->second.size()));

         oss << '&' << pEntryName << '=' << pEntryVal;

         curl_free(pEntryName);
         curl_free(pEntryVal);
      }

      newUrl = oss.str();
   }


   curl_easy_setopt(m_pCurlHandle, CURLOPT_URL, newUrl.c_str());

   pHeaderlist = curl_slist_append(pHeaderlist, "Expect:");
   curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, pHeaderlist);

   string dataStr;

   if ( encode )
   {
      curl_easy_setopt(m_pCurlHandle, CURLOPT_POST, 1);
      curl_easy_setopt(m_pCurlHandle, CURLOPT_POSTFIELDS, pDataEnc);
      dataStr = dataName + '=' + string(pDataEnc);
      curl_easy_setopt(m_pCurlHandle, CURLOPT_POSTFIELDS, dataStr.c_str());
      curl_easy_setopt(m_pCurlHandle, CURLOPT_POSTFIELDSIZE, dataStr.size());
   }
   else
      curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPPOST, pFormpost);


   char pCurlErrorStr[CURL_ERROR_SIZE];
   curl_easy_setopt(m_pCurlHandle, CURLOPT_ERRORBUFFER, pCurlErrorStr);

   // anything to read?
   string ret;
   if ( curl_easy_perform(m_pCurlHandle) == 0 )
      ret = string(m_inBuffer.begin(), m_inBuffer.end());
   else
      ret = "ERROR: " + string(pCurlErrorStr);

   if ( encode )
      curl_free(pDataEnc);
   else
      curl_formfree(pFormpost);

   curl_slist_free_all(pHeaderlist);
   curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPHEADER, NULL); // reset

   return ret;
}

// -----------------------------------------------------------------------------

string HTTPClient::get( const std::string& url )
{

   m_inBuffer.clear();
   char pCurlErrorStr[CURL_ERROR_SIZE];
   curl_easy_setopt(m_pCurlHandle, CURLOPT_ERRORBUFFER, pCurlErrorStr);

   // it's an http get
   curl_easy_setopt(m_pCurlHandle, CURLOPT_HTTPGET, 1);

   // specify URL to get
   curl_easy_setopt(m_pCurlHandle, CURLOPT_URL, url.c_str());

   string ret;
   if ( curl_easy_perform(m_pCurlHandle) == 0 )
      ret = string(m_inBuffer.begin(), m_inBuffer.end());
   else
      ret = "ERROR: " + string(pCurlErrorStr);

   return ret;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

size_t HTTPClient::httpFetch( void *ptr, size_t size, size_t nmemb, void *pData )
{
   vector<char>* pBuf = static_cast< vector<char>* >(pData);
   if ( pBuf == NULL )
      return 0;

   size_t realsize = size * nmemb;
   size_t startSize = pBuf->size();
   
   pBuf->resize(startSize + realsize);
   memcpy( &((*pBuf)[startSize]), ptr, realsize );

   return realsize;
}

// -----------------------------------------------------------------------------
