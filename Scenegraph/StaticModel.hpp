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

#ifndef RTRT_SCENEGRAPH_STATIC_MODEL_HPP
#define RTRT_SCENEGRAPH_STATIC_MODEL_HPP

#include <GL/glew.h>
#include <Core/Types.hpp>
#include "ModelStash.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    class StaticModel;
    typedef boost::shared_ptr< StaticModel > StaticModelPtr;

    /*! A simple static model class */
    class StaticModel : public Stashable< ModelStash >
    {
    public:
      /*! Constructs new empty static model */
      StaticModel();

      /*! Creates new empty static model */
      static StaticModelPtr create() { return StaticModelPtr( new StaticModel()); };

      /*! Construct new static model using specified data arrays */
      StaticModel( const modelStash::VertexArray& vertices, 
		   const modelStash::NormalArray& normals,
		   const modelStash::TextureArray& textures,
		   const modelStash::IndexArray& indices );

      /*! Vertices */
      const modelStash::VertexArray& vertices() const;
      /*! Normals */
      const modelStash::NormalArray& normals() const;
      /*! Textures */
      const modelStash::TextureArray& textures() const;
      /*! Vertex indices */
      const modelStash::IndexArray& indices() const;

      /*! Sets vertices */
      void setVertices( const modelStash::VertexArray& vertices );
      /*! Sets normals */
      void setNormals( const modelStash::NormalArray& normals );
      /*! Sets textures */
      void setTextures( const modelStash::TextureArray& textures );
      /*! Sets vertex indices */
      void setIndices( const modelStash::IndexArray& indices );

      /*! A size of object. Components are: vertices, normals, textures, indices */      
      ModelStashSize allocationSize() const;

      /*! Stashes object */
      void stash( modelStash::VertexType* vertices,
		  modelStash::NormalType* normals,
		  modelStash::TextureType* textures,
		  modelStash::IndexType* indices ) const;

      std::string typeName() const;
      property::ListPropertyPtr properties();

    private:
      modelStash::VertexArray _vertices;
      modelStash::NormalArray _normals;
      modelStash::TextureArray _textures;
      modelStash::IndexArray _indices;

    };
  }
}
#endif
