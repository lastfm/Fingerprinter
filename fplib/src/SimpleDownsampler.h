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

// a very simple downsampler. Note: the resulting range will be between -1 and +1!
// TODO: Replace it with a better one! This sucks! :P

#ifndef __SIMPLE_DOWNSAMPLER_H
#define __SIMPLE_DOWNSAMPLER_H

#include <utility> // for pair

namespace audio
{

// use either float or double!!
template <typename T = float>
class SimpleDownsampler
{
public:

   SimpleDownsampler() : m_hasInit(false), m_eps(0.000001) {}

   void init( int sourceFreq, int sourceNumChannels, 
              float destFreq, int destNumChannels );

   // returns how much it processed for <pDest, pSource>
   inline std::pair<size_t, size_t> 
   downsample( T* pDestPCM_Beg, const T* pDestPCM_End,
               const short* pSourcePCM_Beg, const short* pSourcePCM_End );

private:

   // returns how much it processed for <pDest, pSource>
   // fast version, takes advantage of common source sample rates
   inline std::pair<size_t, size_t> 
   downsample_fixed( T* pDestPCM, size_t destSize,
                     const short* pSourcePCM, size_t sourceSize );

   // returns how much it processed for <pDest, pSource>
   // slow version for weird sample rates
   inline std::pair<size_t, size_t> 
   downsample_float( T* pDestPCM, size_t destSize,
                     const short* pSourcePCM, size_t sourceSize );


private:

   bool     m_hasInit;

   int      m_sourceFreq;
   int      m_sourceNumChannels;
   float    m_destFreq;
   int      m_destNumChannels;

   // used by downsample_fixed
   int      m_leftover;
   size_t   m_leftoverNumFrames;

   // used by downsample_float
   double   m_floatSample;
   double   m_floatFillPos;

   const double m_eps;
};

}

#include "SimpleDownsampler.hpp"

#endif // __SIMPLE_DOWNSAMPLER_H

