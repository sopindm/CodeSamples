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

#include <GL/glew.h>
#include <boost/foreach.hpp>
#include <Eigen/LU>

#include "BVHDataStructure.hpp"
#include "BVHOptimized.hpp"

#include "ModelStash.h"
#include "ModelStash.hpp"

using namespace boost;
using namespace rtrt::cuda;
using namespace rtrt::scenegraph;
using namespace rtrt::scenegraph::modelStash;
using namespace rtrt::bvhOptimized;
using namespace Eigen;

ModelStash::ModelStash(): _bvh( new BVHOptimized() )
{
}

void ModelStash::doAddObject( BaseStashable* object, size_t id )
{
  _cpuData.vertices.insertIndex( id );
  _cpuData.normals.insertIndex( id );
  _cpuData.textures.insertIndex( id );
  _cpuData.indices.insertIndex( id );
  _cpuData.objectIndices.insertIndex( id );
  _cpuData.materialIndices.insertIndex( id );
}

void ModelStash::doRemoveObject( BaseStashable* object, size_t id )
{
  releaseObject( id );
  releaseId( id );
}

void ModelStash::doAllocateObject( BaseStashable* baseObject, size_t id )
{
  StashInterface* object( interfaceCast< StashInterface >( baseObject ));

  ModelStashSize size = object->allocationSize();

  _cpuData.vertices.allocate( id, size.get< 0 >() );
  _cpuData.normals.allocate( id, size.get< 1 >() );
  _cpuData.textures.allocate( id, size.get< 2 >() );
  _cpuData.indices.allocate( id, size.get< 3 >() );
  _cpuData.objectIndices.allocate( id, size.get< 3 >() );
  _cpuData.materialIndices.allocate( id, 1 );

  for( size_t i = _cpuData.indices.offset( id+1 ); i < _cpuData.indices.value().size(); i++ )
    _cpuData.indices.value()[ i ].cwise() += size.get< 0 >();
}

void ModelStash::doFreeObject( BaseStashable* baseObject, size_t id )
{
  _cpuData.materialIndices.free( id );
  releaseObject( id );
}

void ModelStash::releaseObject( size_t id )
{
  size_t size( _cpuData.vertices.size( id ));

  _cpuData.vertices.free( id );
  _cpuData.normals.free( id );
  _cpuData.textures.free( id );
  _cpuData.indices.free( id );
  _cpuData.objectIndices.free( id );

  for( size_t i=_cpuData.indices.offset( id+1 ); i<_cpuData.indices.value().size(); i++ )
    _cpuData.indices.value()[ i ].cwise() -= size;
}

void ModelStash::releaseId( size_t id )
{
  _cpuData.vertices.eraseIndex( id );
  _cpuData.normals.eraseIndex( id );
  _cpuData.textures.eraseIndex( id );
  _cpuData.indices.eraseIndex( id );
  _cpuData.objectIndices.eraseIndex( id ); 

  _cpuData.materialIndices.eraseIndex( id );

  BOOST_FOREACH( uint32_t& oId, _cpuData.objectIndices.value() )
  {
    if( oId > id )
      oId--;
  }
}

void ModelStash::doWriteObject( BaseStashable* object, size_t id, 
				const AttributeList& attributes, 
				const Time& time )
{

  StashInterface* model( interfaceCast< StashInterface >( object ));

  TransformAttributePtr transform( SCENEGRAPH_ATTRIBUTE( Transform, attributes ));

  if( model->isNew( time ) || transform->isNew( time ) )
  {
    setNew();
    _writeObject( model, id, transform->value() );
  }

  MaterialAttributePtr attr( SCENEGRAPH_ATTRIBUTE( Material, attributes ));

  if( attr->isNew( time ))
  {
    setNew();
    _cpuData.materialIndices.at( id ) = attr->value();
  }
}

void ModelStash::_writeObject( StashInterface* model, size_t id, const Eigen::Transform3f& transform )
{
  ModelStashSize realSize( model->allocationSize() );

  assert( _cpuData.vertices.size( id ) == realSize.get< 0 >() &&
	  _cpuData.normals.size( id ) == realSize.get< 1 >() &&
	  _cpuData.textures.size( id ) == realSize.get< 2 >() &&
	  _cpuData.indices.size( id ) == realSize.get< 3 >() &&
	  _cpuData.objectIndices.size( id ) == realSize.get< 3 >() );

  model->stash( &_cpuData.vertices.at( id ),
		&_cpuData.normals.at( id ),
		&_cpuData.textures.at( id ),
		&_cpuData.indices.at( id ) );

  for( size_t i = _cpuData.vertices.offset( id ); i < _cpuData.vertices.offset( id + 1 ); i++ )
    _cpuData.vertices.value()[ i ] = transform * _cpuData.vertices.value()[ i ];

  Transform3f normalTransform( transform.linear().inverse().transpose() );
  
  for( size_t i=_cpuData.normals.offset( id ); i < _cpuData.normals.offset( id+1 ); i++ )     
    _cpuData.normals.value()[ i ] = ( normalTransform * _cpuData.normals.value()[ i ] ).normalized();

  for( size_t i=_cpuData.indices.offset( id ); i < _cpuData.indices.offset( id+1 ); i++ )
    _cpuData.indices.value()[ i ].cwise() += _cpuData.vertices.offset( id );

  for( size_t i=_cpuData.objectIndices.offset( id ); i < _cpuData.objectIndices.offset( id+1 ); i++ )
    _cpuData.objectIndices.value()[ i ] = id;
}

const VertexArray& ModelStash::vertices () const
{
  return _cpuData.vertices.value();
}

const NormalArray& ModelStash::normals () const
{
  return _cpuData.normals.value();
}

const TextureArray& ModelStash::textures () const
{
  return _cpuData.textures.value();
}

const IndexArray& ModelStash::indices () const
{
  return _cpuData.indices.value();
}

const std::vector< uint32_t > ModelStash::objectIndices () const
{
  return _cpuData.objectIndices.value();
}

const std::vector< MaterialID > ModelStash::materialIndices() const
{
  return _cpuData.materialIndices.value();
}

size_t ModelStash::meshesCount() const
{
  return objectIds().size();
}

void ModelStash::drawMesh( size_t index ) const
{
  glBegin ( GL_TRIANGLES );

  for( size_t i = _cpuData.indices.offset( index ); i < _cpuData.indices.offset( index + 1 ); i++ )
  {
    modelStash::IndexType face( indices()[ i ] );

    glNormal3fv( ( normals()[ face[ 0 ]] ).data() );
    glVertex3fv( ( vertices()[ face[ 0 ]] ).data() ); 
    glNormal3fv( ( normals()[ face[ 1 ]] ).data() );
    glVertex3fv( ( vertices()[ face[ 1 ]] ).data() ); 
    glNormal3fv( ( normals()[ face[ 2 ]] ).data() );
    glVertex3fv( ( vertices()[ face[ 2 ]] ).data() ); 
  }

  glEnd();

 // if( index == 0 )
 //   _bvh.draw();
}

MaterialID ModelStash::meshMaterial( size_t index ) const
{
  return _cpuData.materialIndices.at( index );
}

void ModelStash::buildDataStructure()
{
  _bvh->build( _cpuData.vertices.value(), _cpuData.indices.value(), _cpuData.objectIndices.value() );
}

void ModelStash::setupCuda()
{
  _gpuData.reset();
  _gpuData.reset( new GpuData( _cpuData ));

  _bvh->build( _cpuData.vertices.value(), _cpuData.indices.value(), _cpuData.objectIndices.value() );
  _bvh->setupCUDA( _gpuData->vertices );

  cuda::setupGeometry( _gpuData->vertices.devicePtr(), 
		       _gpuData->normals.devicePtr(), 
		       _gpuData->textures.devicePtr(),
		       _bvh->indices().devicePtr(),
		       _bvh->objectIndices().devicePtr(),
		       _gpuData->materialIndices.devicePtr(),
                       _gpuData->textureIndices.devicePtr(),
		       scenegraphID().geometryID,
		       _bvh->indices().size() );
}

ModelStash::GpuData::GpuData( const ModelStash::CpuData& cpuData ):
  vertices( "vertices", cpuData.vertices.value().size() ),
  normals( "normals", cpuData.normals.value().size() ),
  textures( "textures", cpuData.textures.value().size() ),
  materialIndices( "materialIndices", cpuData.materialIndices.value().size() ),
  textureIndices( "textureIndices", cpuData.materialIndices.value().size() )
{
  vertices.write( reinterpret_cast< const float3* >( cpuData.vertices.value().data() ));
  normals.write( reinterpret_cast< const float3* >( cpuData.normals.value().data() ));
  textures.write( reinterpret_cast< const float2* >( cpuData.textures.value().data() ));
  materialIndices.write( reinterpret_cast< const uint* >( cpuData.materialIndices.value().data() ));

  Array2i texIndices( cpuData.materialIndices.value().size() );
  for( size_t id = 0; id < cpuData.materialIndices.value().size(); id++ )
  {
    int texOffset = -1;

    if( cpuData.textures.size( id ) != 0 )
      texOffset = cpuData.textures.offset( id );
    texIndices[ id ] = Vector2i( cpuData.vertices.offset( id ), texOffset );
  }

  textureIndices.write( reinterpret_cast< const int2* >( texIndices.data() ));
}

void ModelStash::intersect( float3* origins, 
			    float3* directions,
			    float* times,
			    uint2* intersections,
			    uint rays )
{
  _bvh->intersect( origins, directions, times, intersections, rays );

}

void ModelStash::intersectShadows( float3* origins, 
				   float3* directions,
				   float* times,
				   float3* radiances,
				   uint rays )
{
  _bvh->intersectAny( origins, directions, times, radiances, rays );
}

const rtrt::DataStructurePtr& ModelStash::dataStructure() const
{
  return _bvh;
}

void ModelStash::setDataStructure( const DataStructurePtr& structure )
{
  _bvh = structure;
  setupCuda();
}
