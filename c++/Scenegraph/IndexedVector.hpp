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

#ifndef RTRT_SCENEGRAPH_INDEXED_VECTOR_HPP
#define RTRT_SCENEGRAPH_INDEXED_VECTOR_HPP

#include "OffsetArray.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    /*! A class of vector supporting storing of multiple objects in one array */
    template< class Vector >
    class IndexedVector
    {
    public:
      IndexedVector();

      /*! Inserts new object in array without allocating space to it */
      void insertIndex( size_t index );
      /*! Erases object from array */
      void eraseIndex( size_t index );

      /*! Allocates space for object */
      void allocate( size_t index, size_t size );
      /*! Frees object's space */
      void free( size_t index );

      /*! An offset for object in array */
      size_t offset( size_t id ) const;
      /*! A size of object */
      size_t size( size_t id ) const;

      /*! Inner vector */
      Vector& value();
      /*! Inner vector */
      const Vector& value() const;

      /*! Inner offset vector */
      std::vector< size_t >& offsets();
      /*! Inner offset vector */
      const std::vector< size_t >& offsets() const;

      /*! First value of object in vector */
      typename Vector::value_type& at( size_t index ); 
      /*! First value of object in vector */
      const typename Vector::value_type& at( size_t index ) const;

    private:
      
      Vector _vector;
      OffsetArray< size_t > _offsets;
    };
  }
}

#include "IndexedVector_impl.hpp"
#endif
