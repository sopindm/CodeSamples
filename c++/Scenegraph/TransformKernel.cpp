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

#include <Property/PropertyBuilder.hpp>
#include "TransformKernel.hpp"

using namespace Eigen;
using namespace rtrt::scenegraph;
using namespace rtrt::property;

TransformKernel::TransformKernel(): _translation( 0, 0, 0 ),
				    _rotation( 0, 0, 0 ),
				    _scale( 1, 1, 1 )
{
}

TransformKernelPtr TransformKernel::createTranslation( float dx, float dy, float dz )
{
  TransformKernelPtr ret( new TransformKernel );
  ret->translate( dx, dy, dz );

  return ret;
}

TransformKernelPtr TransformKernel::createTranslation( const Eigen::Vector3f& delta )
{
  TransformKernelPtr ret( new TransformKernel );
  ret->translate( delta );

  return ret;
}

TransformKernelPtr TransformKernel::createRotation( float dx, float dy, float dz )
{
  TransformKernelPtr ret( new TransformKernel );
  ret->rotate( dx, dy, dz );

  return ret;
}

TransformKernelPtr TransformKernel::createRotation( const Eigen::Vector3f& delta )
{
  TransformKernelPtr ret( new TransformKernel );
  ret->rotate( delta );

  return ret;
}

TransformKernelPtr TransformKernel::createScaling( float dx, float dy, float dz )
{
  TransformKernelPtr ret( new TransformKernel );
  ret->scale( dx, dy, dz );

  return ret;
}

TransformKernelPtr TransformKernel::createScaling( const Eigen::Vector3f& delta )
{
  TransformKernelPtr ret( new TransformKernel );
  ret->scale( delta );

  return ret;
}

void TransformKernel::rotate( float angleX, float angleY, float angleZ )
{
  rotate( Vector3f( angleX, angleY, angleZ ));
}

void TransformKernel::rotate( const Vector3f& rotation )
{
  _rotation += rotation;
  renew();
}

void TransformKernel::translate( float dx, float dy, float dz )
{
  translate( Vector3f( dx, dy, dz ));
} 

void TransformKernel::translate( const Vector3f& delta )
{
  _translation += delta;
  renew();
}

void TransformKernel::scale( float sx, float sy, float sz )
{
  scale( Vector3f( sx, sy, sz ));
}

void TransformKernel::scale( const Vector3f& value )
{
  _scale.cwise() *= value;
  renew();
}

TransformKernel::TransformPtr TransformKernel::transform() const
{
  TransformPtr ret( new Eigen::Transform3f( Eigen::Transform3f::Identity() ));

  *ret *= Translation3f( _translation.x(), _translation.y(), _translation.z() );
  *ret *= Scaling3f( _scale.x(), _scale.y(), _scale.z() );
  
  *ret *= AngleAxisf( _rotation.z(), Vector3f::UnitZ() ) *
    AngleAxisf( _rotation.y(), Vector3f::UnitY() ) *
    AngleAxisf( _rotation.x(), Vector3f::UnitX() );

  return ret;
}

void TransformKernel::update( double )
{
  KERNEL_SET_ATTRIBUTE( *transform(), Transform );
}

std::string TransformKernel::typeName() const
{
  return "Transform";
}

ListPropertyPtr TransformKernel::properties()
{
  return buildListProperty( "kernel", 
			    buildProperty( "translation", _translation ),
			    buildProperty( "rotation", _rotation ),
			    buildProperty( "scale", _scale ) );
}
