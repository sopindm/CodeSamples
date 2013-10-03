/*
   -----------------------------------------------------------------------------
   |                 R T R T    S C E N E G R A P H   L I B R A R Y                 |
   -----------------------------------------------------------------------------
                              
                             
   Copyright (c) 2010 - 2012  Dmitry Sopin ( sopindm @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RTRT_SCENEGRAPH_OFFSET_ARRAY_HPP
#define RTRT_SCENEGRAPH_OFFSET_ARRAY_HPP

#include <vector>

namespace rtrt
{
  namespace scenegraph
  {
    /*! Class storing offsets for objects in stashes */
    template< class T >
    class OffsetArray
    {
    public:
      /*! Creates new array */
      OffsetArray( T zero ) { _offsets.push_back( zero ); };

      /*! Inserts new element in array */
      void insert( size_t index );
      /*! Erases element from array */
      void erase( size_t index );

      /*! Updates indices after allocation of space for object */
      void allocate( size_t index, T size );
      /*! Updates indices after object frees it's memory */
      void free( size_t index );

      /*! An offset of object */
      T offset( size_t index ) const;
      /*! A size of allocated space for object */
      T size( size_t index ) const;

      /*! Inner vector */
      std::vector< T >& value();
      /*! Inner vector */
      const std::vector< T >& value() const;

      /*! Elements count in array */
      size_t length() const { return _offsets.size() - 1; };

    private:
      OffsetArray( const OffsetArray& );
      void operator=( const OffsetArray& );
      
      std::vector< T > _offsets;

    };
  }
}

#include "OffsetArray_impl.hpp"
#endif
