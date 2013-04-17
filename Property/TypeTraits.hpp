/*
   -----------------------------------------------------------------------------
   |                 R T R T    P R O P E R T Y   L I B R A R Y                 |
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

#ifndef RTRT_PROPERTY_TYPETRAITS_HPP
#define RTRT_PROPERTY_TYPETRAITS_HPP

#include <vector>
#include <Core/Types.hpp>

namespace rtrt
{
  namespace property
  {
    template< class T >
    struct Type
    {
      typedef std::vector< T > VectorType;
      static const bool haveRawAccess = false;
    };

    template<>
    struct Type< int >
    {
      typedef std::vector< int > VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< uint32_t >
    {
      typedef std::vector< uint32_t > VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< float >
    {
      typedef std::vector< float > VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< Eigen::Vector2f >
    {
      typedef Array2f VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< Eigen::Vector3f >
    {
      typedef Array3f VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< Eigen::Vector4f >
    {
      typedef Array4f VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< Eigen::Vector3i >
    {
      typedef Array3i VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< Eigen::Vector4i >
    {
      typedef Array4i VectorType;
      static const bool haveRawAccess = true;
    };

    template<>
    struct Type< Eigen::Transform3f >
    {
      typedef std::vector< Eigen::Transform3f, Eigen::aligned_allocator< Eigen::Transform3f > > VectorType;
      static const bool haveRawAccess = true;
    };
  }
}
#endif
