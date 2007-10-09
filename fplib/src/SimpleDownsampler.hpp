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
 *                                                                         *
 ***************************************************************************/

#include <stdexcept>
#include <limits>
#include <cassert>
#include <sstream>
#include <cmath>

#undef max

namespace audio
{

template <typename T>
void
SimpleDownsampler<T>::init( int sourceFreq, int sourceNumChannels, 
                            float destFreq, int destNumChannels)
{
   m_sourceFreq = sourceFreq;
   m_sourceNumChannels = sourceNumChannels;
   m_destFreq = destFreq;
   m_destNumChannels = destNumChannels;

   if ( destNumChannels != 1 && 
        destNumChannels != 2 )
   {
      throw std::runtime_error("Unsupported number of channels");
   }

   m_leftover = 0;
   m_leftoverNumFrames = 0;

   m_floatSample = 0;
   m_floatFillPos = 0;

   m_hasInit = true;
}


// -----------------------------------------------------------------------------

template <typename T>
std::pair<size_t, size_t> 
SimpleDownsampler<T>::downsample( T* pDestPCM_Beg, const T* pDestPCM_End,
                                  const short* pSourcePCM_Beg, const short* pSourcePCM_End )
{
   if ( !m_hasInit )
      throw std::runtime_error("Please call SimpleDownsampler::init()  before SimpleDownsampler::downsample()!");

   switch (m_sourceFreq)
   {
   case 11025:
   case 22050:
   case 44100:
      return downsample_fixed( pDestPCM_Beg, pDestPCM_End-pDestPCM_Beg, 
                               pSourcePCM_Beg, pSourcePCM_End-pSourcePCM_Beg );
      break;
   default:
      return downsample_float( pDestPCM_Beg, pDestPCM_End-pDestPCM_Beg, 
                               pSourcePCM_Beg, pSourcePCM_End-pSourcePCM_Beg );
      break;
   }

   return std::make_pair(0,0);
}

// -----------------------------------------------------------------------------

template <typename T>
std::pair<size_t, size_t> 
SimpleDownsampler<T>::downsample_fixed( T* pDestPCM, size_t destSize, 
                                        const short* pSourcePCM, size_t sourceSize )
{
   if ( destSize == 0 || sourceSize == 0 )
      return std::make_pair(0,0);

   int downsample_ratio = (m_sourceFreq / static_cast<int>(m_destFreq)) * (m_sourceNumChannels / m_destNumChannels);
   if ( downsample_ratio <= 0 )
   {
      throw std::runtime_error("stream samplerate is too low, upsampling is not supported!");
   }

   static const T shortMaxAndDownSample = static_cast<T>( std::numeric_limits<short>::max() ) * downsample_ratio;
   size_t remainingToFill = 0;
   size_t remainingDest = 0;

   int tmpSample;
   if ( m_leftoverNumFrames != 0 )
   {
      // there are some frames which were left over last time we called downsample_fixed
      remainingToFill = downsample_ratio - m_leftoverNumFrames;

      tmpSample = m_leftover;
      for ( size_t i = 0; i < remainingToFill; ++i )
         tmpSample += pSourcePCM[i];

      *pDestPCM = static_cast<T>(tmpSample) / shortMaxAndDownSample;

      ++pDestPCM;
      --destSize;
      remainingDest = 1;

      pSourcePCM += remainingToFill;
      sourceSize -= remainingToFill;
   }

   // how much is available in the source at the downsampled ratio
   size_t availableSource = sourceSize / downsample_ratio;
   size_t samples_to_use = std::min( destSize, availableSource );

   switch (downsample_ratio)
   {
   case 1:
      for (size_t i = 0; i < samples_to_use; ++i)
         pDestPCM[i] = static_cast<T>(pSourcePCM[i]) / shortMaxAndDownSample;
      break;

   case 2:
      for (size_t i = 0; i < samples_to_use; ++i)
      {
         tmpSample = pSourcePCM[2*i]   +
                     pSourcePCM[2*i+1];
         //tmpSample /= 2;

         pDestPCM[i] = (static_cast<T>(tmpSample) / shortMaxAndDownSample);
      }
      break;

   case 4:
      //for ( size_t i = 0; pDestPCMIt != pDestPCMEnd; ++pDestPCMIt, ++i )
      for (size_t i = 0; i < samples_to_use; ++i)
      {
         tmpSample = pSourcePCM[4*i]   +
                     pSourcePCM[4*i+1] +
                     pSourcePCM[4*i+2] +
                     pSourcePCM[4*i+3];

         //tmpSample /= 4;
         //*pDestPCMIt = (static_cast<float>(tmpSample) / shortMax);
         pDestPCM[i] = (static_cast<T>(tmpSample) / shortMaxAndDownSample);
      }
      break;

   case 8:

      //for ( size_t i = 0; pDestPCMIt != pDestPCMEnd; ++pDestPCMIt, ++i )
      for (size_t i = 0; i < samples_to_use; ++i)
      {
         tmpSample = pSourcePCM[8*i]   +
                     pSourcePCM[8*i+1] +
                     pSourcePCM[8*i+2] +
                     pSourcePCM[8*i+3] +
                     pSourcePCM[8*i+4] +
                     pSourcePCM[8*i+5] +
                     pSourcePCM[8*i+6] +
                     pSourcePCM[8*i+7];

         //tmpSample /= 8;
         //        *pDestPCMIt = (static_cast<float>(tmpSample) / shortMax);
         pDestPCM[i] = (static_cast<T>(tmpSample) / shortMaxAndDownSample);
      }
      break;

   case 16: 
      //      for ( size_t i = 0; pDestPCMIt != pDestPCMEnd; ++pDestPCMIt, ++i )
      for (size_t i = 0; i < samples_to_use; ++i)
      {

         tmpSample = pSourcePCM[16*i]   +
                     pSourcePCM[16*i+1] +
                     pSourcePCM[16*i+2] +
                     pSourcePCM[16*i+3] +
                     pSourcePCM[16*i+4] +
                     pSourcePCM[16*i+5] +
                     pSourcePCM[16*i+6] +
                     pSourcePCM[16*i+7] +
                     pSourcePCM[16*i+8] +
                     pSourcePCM[16*i+9] +
                     pSourcePCM[16*i+10]+
                     pSourcePCM[16*i+11]+
                     pSourcePCM[16*i+12]+
                     pSourcePCM[16*i+13]+
                     pSourcePCM[16*i+14]+
                     pSourcePCM[16*i+15];

         //tmpSample /= 16;
         pDestPCM[i] = (static_cast<T>(tmpSample) / shortMaxAndDownSample);
      }
      break;

   default:
      assert(0);
      break;
   };

   m_leftover = 0;

   // add the rest to the leftover. 
   // That's only happening when the destination requested more that's in
   // the source, and the source size it's not a multiple of downsample_ratio

   // let's see how many samples are left
   m_leftoverNumFrames = (sourceSize - (samples_to_use*downsample_ratio)) % downsample_ratio ;
   if ( (destSize != availableSource) && 
        ( m_leftoverNumFrames != 0 ) )
   {

      size_t currPos = samples_to_use * downsample_ratio;
      for ( size_t i = 0; i < m_leftoverNumFrames; ++i )
         m_leftover += pSourcePCM[currPos+i];
   }

   return std::make_pair( samples_to_use+remainingDest, 
                          samples_to_use*downsample_ratio + m_leftoverNumFrames + remainingToFill);
}

// -----------------------------------------------------------------------------

template <typename T>
std::pair<size_t, size_t> 
SimpleDownsampler<T>::downsample_float( T* pDestPCM, size_t destSize,
                                        const short* pSourcePCM, size_t sourceSize )
{
   if ( destSize == 0 || sourceSize == 0 )
      return std::make_pair(0,0);

   if (m_sourceFreq < m_destFreq)
   {
      std::ostringstream oss;
      oss << "Samplerate has to be at least " << m_destFreq << "!";
      throw std::runtime_error(oss.str());
   }

   if ( sourceSize % m_sourceNumChannels != 0 )
      throw std::runtime_error("The size of the source buffer must be a multiple of the number of channels!");

   double diff;
   double tmpSample;
   int sourceBlocSize;

   const double shortMax = static_cast<double>( std::numeric_limits<short>::max() ) ;
   double downsample_ratio = (static_cast<double>(m_sourceFreq) / static_cast<double>(m_destFreq));
   
   const short* pSourceNextBlocEndIt = NULL;
   const short* pSourcePCMIt = pSourcePCM;
   const short* pSourcePCMEndIt = pSourcePCM + sourceSize;

   size_t i = 0;

   do
   {
      if ( m_sourceNumChannels == 1 )
      {
         for ( ; m_floatFillPos +1 < downsample_ratio; ++m_floatFillPos )
            m_floatSample += *(pSourcePCMIt++);
      }
      else
      {
         for ( ; m_floatFillPos +1 < downsample_ratio; ++m_floatFillPos )
            m_floatSample += (*(pSourcePCMIt++) + *(pSourcePCMIt++)) / 2;
      }

      if ( m_floatFillPos != downsample_ratio )
      {
         //--m_floatFillPos;
         diff = downsample_ratio - m_floatFillPos;

         assert(diff <= 1.0);

         if ( m_sourceNumChannels == 1 )
            tmpSample = *(pSourcePCMIt++);
         else
            tmpSample = (*(pSourcePCMIt++) + *(pSourcePCMIt++)) / 2;

         // what remains
         m_floatSample += tmpSample * diff;

         // here's the downsampled frame
         pDestPCM[i] = static_cast<T>( m_floatSample / (downsample_ratio * shortMax) );

         // that's for the next frame
         m_floatFillPos = 1 - diff;
         m_floatSample = tmpSample * m_floatFillPos;
      }
      else
      {
         pDestPCM[i] = static_cast<T>( m_floatSample / (downsample_ratio * shortMax) );
         m_floatSample = 0;
         m_floatFillPos = 0;
      }

      ++i;
      sourceBlocSize = static_cast<int>( 
                       ceil( downsample_ratio * m_sourceNumChannels - m_floatFillPos) );
      sourceBlocSize += ( sourceBlocSize % m_sourceNumChannels );

      pSourceNextBlocEndIt = pSourcePCMIt + sourceBlocSize;

   } while ( i < destSize && pSourceNextBlocEndIt <= pSourcePCMEndIt );


   if ( pSourceNextBlocEndIt >= pSourcePCMEndIt )
      pSourceNextBlocEndIt = pSourcePCMEndIt;

   int numLeft = static_cast<int>(pSourceNextBlocEndIt - pSourcePCMIt);
   if (  numLeft > 0 && numLeft < sourceBlocSize )
   {
      // do the remaining of the source samples
      if ( m_sourceNumChannels == 1 )
      {
         for (; pSourcePCMIt != pSourceNextBlocEndIt; ++m_floatFillPos )
            m_floatSample += *(pSourcePCMIt++);
      }
      else
      {
         for (; pSourcePCMIt != pSourceNextBlocEndIt; ++m_floatFillPos )
            m_floatSample += (*(pSourcePCMIt++) + *(pSourcePCMIt++)) / 2;
      }
   }

   return std::make_pair( i, 
                          pSourcePCMIt - pSourcePCM);
}

// -----------------------------------------------------------------------------

} // end of namespace audio

// -----------------------------------------------------------------------------
