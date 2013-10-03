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

#ifndef RTRT_SCENEGRAPH_MODELSTASH_HPP
#define RTRT_SCENEGRAPH_MODELSTASH_HPP

#include <GL/glew.h>
#include <map>
#include <boost/tuple/tuple.hpp>
#include <Eigen/Core>
#include <Core/Types.hpp>
#include <Cuda/Buffer.hpp>

#include "Materials/Material.hpp"
#include "IndexedVector.hpp"
#include "BaseStash.hpp"
#include "GeometryStash.hpp"
#include "DataStructure.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    class ModelStash;

    typedef boost::tuple< size_t, size_t, size_t, size_t > ModelStashSize;

    namespace modelStash
    {
      typedef Eigen::Vector3f VertexType;
      typedef Array3f VertexArray;
      typedef Eigen::Vector3f NormalType;
      typedef Array3f NormalArray;
      typedef Eigen::Vector2f TextureType;
      typedef Array2f TextureArray;
      typedef Eigen::Vector3i IndexType;
      typedef Array3i IndexArray;

      /*! Stash inferface for geometry objects */
      class StashInterface : public BaseStashInterface
      {
      public:
	/*! A size of object. Components are: vertices, normals, textures, indices */
	virtual ModelStashSize allocationSize() const = 0;

	/*! Stashes object */
	virtual void stash( VertexType* vertices,
			    NormalType* normals,
			    TextureType* textures,
			    IndexType* indices ) const = 0;

      };
    }

    /*! Stash for polygonal models */
    class ModelStash : public Stash< modelStash::StashInterface >, public GLGeometry, public CudaGeometry
    {
    public:
      static std::string stashName() { return "ModelStash"; };

      /*! Stash's name */
      ModelStash();
      
      /*! Array of vertices */
      const modelStash::VertexArray& vertices () const;
      /*! Array of normals */
      const modelStash::NormalArray& normals () const;
      /*! Array of textures */
      const modelStash::TextureArray& textures () const;
      /*! Array of indices */
      const modelStash::IndexArray& indices () const;
      /*! Array of object indices */
      const std::vector< uint32_t > objectIndices () const;      
      /*! Array of material indices for objects */
      const std::vector< MaterialID > materialIndices() const;

      size_t meshesCount() const;

      void drawMesh( size_t index ) const;
      MaterialID meshMaterial( size_t index ) const;

      void setupCuda();
    
      void intersect( float3* origins,
		      float3* directions,
		      float* times,
		      uint2* objectIndices,
		      uint rays );

      void intersectShadows( float3* origins,
			     float3* directions,
			     float* times,
			     float3* radiances,
			     uint rays );

      std::string typeName() const { return stashName(); };

      void buildDataStructure();

      const DataStructurePtr& dataStructure() const;
      void setDataStructure( const DataStructurePtr& structure );

    protected:
      void doAddObject( BaseStashable* object, size_t );
      void doRemoveObject( BaseStashable* object, size_t id );

      void doAllocateObject( BaseStashable* object, size_t id );
      void doFreeObject( BaseStashable* object, size_t id );
      void doWriteObject( BaseStashable* object, size_t id, const AttributeList& attributes, const Time& time );

    private:	
      ModelStash( const ModelStash& stash );
      const ModelStash& operator=( const ModelStash& stash );

      void _writeObject( modelStash::StashInterface* model, size_t id, const Eigen::Transform3f& transform );
      
      void releaseObject( size_t id );
      void releaseId( size_t id );

      struct CpuData
      {
        IndexedVector< modelStash::VertexArray > vertices;
        IndexedVector< modelStash::NormalArray > normals;
        IndexedVector< modelStash::TextureArray > textures;
        IndexedVector< modelStash::IndexArray > indices;
        IndexedVector< std::vector< uint32_t > > objectIndices;
        IndexedVector< std::vector< MaterialID > > materialIndices;
      };

      struct GpuData
      {
        GpuData( const CpuData& data );

        rtrt::cuda::DeviceBuffer< float3 > vertices;
        rtrt::cuda::DeviceBuffer< float3 > normals;
        rtrt::cuda::DeviceBuffer< float2 > textures;
        rtrt::cuda::DeviceBuffer< uint32_t > materialIndices;
        rtrt::cuda::DeviceBuffer< int2 > textureIndices;
      };

      CpuData _cpuData;
      boost::shared_ptr< GpuData > _gpuData;

      DataStructurePtr _bvh;

    };
    typedef boost::shared_ptr< ModelStash > ModelStashPtr;
    
  }
}
#endif

