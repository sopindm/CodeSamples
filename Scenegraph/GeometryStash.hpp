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

#ifndef RTRT_SCENEGRAPH_GEOMETRYSTASH_HPP
#define RTRT_SCENEGRAPH_GEOMETRYSTASH_HPP

#include <vector>

#include <Cuda/Buffer.hpp>
#include "Materials/Material.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    /*! Base interface for geometry stashes */
    class GeometryStash
    {
    public:
      virtual ~GeometryStash() {};

    };

    typedef boost::shared_ptr< GeometryStash > GeometryStashPtr;
    typedef std::vector< GeometryStashPtr > GeometryVector;

    /*! OpenGL geometry interface */
    class GLGeometry : virtual public GeometryStash
    {
    public:
      /*! Meshes in stash */
      virtual size_t meshesCount() const = 0;

      /*! Draws mesh using OpenGL */
      virtual void drawMesh( size_t index ) const = 0;
      /*! Material of mesh */
      virtual MaterialID meshMaterial( size_t index ) const = 0;

    };

    class CudaGeometry : virtual public GeometryStash
    {
    public:
      virtual void setupCuda() = 0;

      virtual void intersect( float3* origins,
			      float3* directions,
			      float* intersectionDepths,
			      uint2* intersectionIds,
			      uint rays ) = 0;

      virtual void intersectShadows( float3* origins,
				     float3* directions,
				     float* times,
				     float3* radiances,
				     uint rays ) = 0;

    };
  }
}
#endif
