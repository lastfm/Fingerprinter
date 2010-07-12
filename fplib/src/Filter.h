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
*                                                                          *
* Part of this code is based on the work of Y. Ke, D. Hoiem, and           *
* R. Sukthankar - "Computer Vision for Music Identification",              *
* in Proceedings of Computer Vision and Pattern Recognition, 2005.         *
* See also http://www.cs.cmu.edu/~yke/musicretrieval/                      *
***************************************************************************/

#ifndef __FILTER_H
#define __FILTER_H

namespace fingerprint
{

struct Filter 
{
	/// Constructs a new filter with id.
	Filter(unsigned int id, float threshold, float weight);
   
   unsigned int id; //< filter id
	unsigned int wt; //< time width
	unsigned int first_band; //< first band
	unsigned int wb; //< band width
	unsigned int filter_type; //< filter type
	
	float threshold; //< threshold for filter
	float weight; //< filter weight

   // number of frames in time
   static const unsigned int KEYWIDTH = 100;
   // number of bands to divide the signal (log step)
   static const unsigned int NBANDS = 33;
};

}

#endif // __FILTER_H
