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

#include <Property/VectorProperty.hpp>
#include "StaticModel.hpp"

using namespace rtrt;
using namespace rtrt::property;
using namespace rtrt::scenegraph;
using namespace rtrt::scenegraph::modelStash;
using namespace boost;
using namespace Eigen;

StaticModel::StaticModel(): _vertices(),
			    _normals(),
			    _textures(),
			    _indices()
{
}

StaticModel::StaticModel( const VertexArray& vertices, 
			  const NormalArray& normals,
			  const TextureArray& textures,
			  const IndexArray& indices ): 
  _vertices( vertices ),
  _normals( normals ),
  _textures( textures ),
  _indices( indices )
{
}

template< class T, class U >
void makeVector( const T& source, U& dest )
{
  dest.resize( source.size() );
  copyVector( source, dest );
}

template< class T, class U >
void copyVector( const T& vector, U& destVector )
{
  for( size_t i=0; i< vector.size(); i++ )
    destVector[ i ] = vector[ i ];
}

const Array3f& StaticModel::vertices() const
{
  return _vertices;
}

void StaticModel::setVertices( const Array3f& vertices )
{
  setNew();
  makeVector( vertices, _vertices );
}

const NormalArray& StaticModel::normals() const
{
  return _normals;
}

void StaticModel::setNormals( const NormalArray& normals )
{
  setNew();
  makeVector( normals, _normals );
}

const Array2f& StaticModel::textures() const
{
  return _textures;
}

void StaticModel::setTextures( const Array2f& textures )
{
  setNew();
  makeVector( textures, _textures );
}

const IndexArray& StaticModel::indices() const
{
  return _indices;
}

void StaticModel::setIndices( const IndexArray& indices )
{
  setNew();
  makeVector( indices, _indices );
}

ModelStashSize StaticModel::allocationSize() const
{
  return make_tuple( _vertices.size(), _normals.size(), _textures.size(), _indices.size() );
}

void StaticModel::stash( VertexType* vertices, NormalType* normals, TextureType* textures, IndexType* indices ) const
{
  copyVector( _vertices, vertices );
  copyVector( _normals, normals );
  copyVector( _textures, textures );
  copyVector( _indices, indices );
}

std::string StaticModel::typeName() const
{
  return "StaticModel";
}

ListPropertyPtr StaticModel::properties() 
{
  return buildListProperty( "StaticModel", 
			    buildProperty< VertexType >( "vertices", "vector", _vertices ),
			    buildProperty< NormalType >( "normals", "vector", _normals ),
			    buildProperty< TextureType >( "textures", "vector", _textures ),
			    buildProperty< IndexType >( "indices", "vector", _indices ) );
}
