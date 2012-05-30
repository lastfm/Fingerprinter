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

#ifndef LASTFM_LIBFPLIB_FLOAT_AVERAGE_H__
#define LASTFM_LIBFPLIB_FLOAT_AVERAGE_H__

//#include <deque>
#include <limits>
#include "CircularArray.h"

template <typename T>
class FloatingAverage
{
public:
   FloatingAverage(size_t size)
   {
      m_values.resize(size);
      m_valIt = m_values.head();
      m_sum = 0;
      m_bufferFilled = false;
   }

   void purge()
   {
      m_sum = 0;
      const T* pCircularBuffer = m_values.get_buffer();
      const int size = m_values.size();

      for ( int i = 0; i < size; ++i )
         m_sum += pCircularBuffer[i];
   }

   void add(const T& value)
   {
      m_sum += value;

      if ( m_bufferFilled )
      {
         m_sum -= *m_valIt;
         *m_valIt = value;
         ++m_valIt;
      }
      else
      {
         *m_valIt = value;
         ++m_valIt;
         if ( m_valIt == m_values.head() )
            m_bufferFilled = true;
      }
   }

   T getAverage() const
   {
      if ( !m_bufferFilled )
         return m_sum / (m_valIt - m_values.head());
      else
         return m_sum / m_values.size();
   }

   T getError() const
   {
      T real_sum = 0;
      const T* pCircularBuffer = m_values.get_buffer();
      for ( int i = 0; i < size; ++i )
         real_sum += pCircularBuffer[i];
      return abs(real_sum - m_sum) / this->size();
   }

   size_t size() const
   {
      return m_values.size();
   }

   void clear()
   {
      m_bufferFilled = false;
      m_values.zero_fill();
      m_valIt = m_values.head();
      m_sum = 0;
   }

private:
   //std::deque<T> m_values;
   CircularArray<T> m_values;
   typename CircularArray<T>::iterator m_valIt;

   bool m_bufferFilled;
   T m_sum;
};

#endif // __FLOAT_AVERAGE_H__
