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

#include <cuda.h>
#include <cuda_runtime.h>
#include <LocalCamera.hpp>
#include <WorldCamera.hpp>
#include <Cuda/Vectors.hpp>
#include <Property/PropertyBuilder.hpp>
#include <Property/VectorProperty.hpp>
#include "Rendering/GLRender.hpp"
#include "Camera.h"
#include "Camera.hpp"

using std::string;
using namespace boost;
using namespace Eigen;
using namespace rtrt;
using namespace rtrt::property;
using namespace rtrt::scenegraph;
using namespace rtrt::cuda;

Camera::Camera(): _baseCamera( new LocalCamera )
{
}

Camera::Camera( const Transform3f& transform ): _baseCamera( new LocalCamera( transform ))
{
}

Camera::Camera( const BaseCameraPtr& camera ): _baseCamera( camera )
{
}

CameraPtr Camera::create()
{
  return CameraPtr( new Camera() );
}

CameraPtr Camera::create( const Transform3f& transform )
{
  return CameraPtr( new Camera( transform ));
}

CameraPtr Camera::create( const BaseCameraPtr& camera )
{
  return CameraPtr( new Camera( camera ));
}

void Camera::setCamera( const BaseCameraPtr& camera )
{
  setNew();
  _baseCamera = camera;
}

void Camera::setLocal()
{
  _baseCamera.reset( new LocalCamera( _baseCamera->transform() ));
}

void Camera::setWorld()
{
  _baseCamera.reset( new WorldCamera( _baseCamera->transform() ));
}

Transform3f Camera::transform() const
{
  return _baseCamera->transform();
}

void Camera::setTransform( const Eigen::Transform3f& transform )
{
  setNew();
  _baseCamera->setTransform( transform );
}

Vector3f Camera::position() const
{
  return _baseCamera->position();
}

void Camera::setPosition( const Vector3f& position )
{
  setNew();
  _baseCamera->setPosition( position );
}

void Camera::translate( const Vector3f& delta )
{
  setNew();
  _baseCamera->translate( delta );
}

void Camera::rotate( const Vector3f& angle )
{
  setNew();
  _baseCamera->rotate( angle );
}

Vector3f Camera::view() const
{
  return _baseCamera->view();
}

Vector3f Camera::up() const
{
  return _baseCamera->up();
}

Vector3f Camera::right() const
{
  return _baseCamera->right();
}

void Camera::update( double time )
{
  while( time < 0 )
    time += 30;

  while( time > 30 )
    time -= 30;

  time /= 30;

  if( _baseCamera->navigate( time ) )
    setNew();

  KERNEL_SET_ATTRIBUTE( _baseCamera->transform(), Transform );
}

void Camera::lookAt( const Vector3f& eye, const Vector3f& view, const Vector3f& up ) const
{
  glLoadIdentity();

  gluLookAt( eye.x(), eye.y(), eye.z(),
	     view.x(), view.y(), view.z(),
	     up.x(), up.y(), up.z() );
}

void Camera::renderGL( const GLRender& render ) const
{
  setupGL();
  render.drawScene();
}

void Camera::setupGL() const
{
  glMatrixMode( GL_PROJECTION );
  glLoadIdentity ( );

  gluPerspective ( 45,
                   viewport()->width() / static_cast< GLfloat >( viewport()->height() ),
		   0.1,
		   1000.0 );

  glMatrixMode( GL_MODELVIEW );

  glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glLoadIdentity();

  lookAt( baseTransform() * position(),
	  baseTransform() * ( position() + view() ),
	  baseTransform().linear() * up() );
}

void Camera::stash( ScenegraphCameraPtr& camera ) const
{
  camera = dynamic_pointer_cast< CameraStash::StashInterface >( node()->kernel() );
  assert( camera != NULL );
}

void Camera::generateRays( DeviceBuffer< float3 >& origins, 
			   DeviceBuffer< float3 >& directions, 
			   const rtrt::cuda::DeviceBuffer< int >& baseIndices,
			   rtrt::cuda::DeviceBuffer< int >& screenIndices,
			   bool randomize,
			   DeviceBuffer< char >& states,
			   int raysPerPixel,
			   size_t raysCount ) const
{
  float aspect = static_cast< float >( viewport()->width() ) / viewport()->height();

  cuda::generateRays( origins.devicePtr(), 
		      directions.devicePtr(), 
		      baseIndices.devicePtr(),
		      screenIndices.devicePtr(),
		      viewport()->width(), 
		      viewport()->height(),
		      cudaVector( static_cast< Vector3f >( baseTransform() * position() )),
		      cudaVector( static_cast< Vector3f >( baseTransform().linear() * view() )),
		      cudaVector( static_cast< Vector3f >( baseTransform().linear() * up() )),
		      cudaVector( static_cast< Vector3f >( baseTransform().linear() * right() )),
                      cudaVector( tan( M_PI / 8. * aspect ) , tan( M_PI / 8. ) ),
		      randomize,
		      states.devicePtr(),
		      raysPerPixel,
		      raysCount );
}

std::string Camera::typeName() const
{
  return "Camera";
}

template< class Accessor >
class ElementProperty: public FloatProperty
{
public:
  static PropertyPtr create( const string& name, Accessor accessor, int position )
  {
    PropertyPtr ret( new ElementProperty( name, accessor, position ));
    ret->setSelfLink( ret );

    return ret;
  }

  float get() const { return _accessor.get( _position ); };
  void set( const float& value ) { _accessor.set( value, _position ); };

  bool isReadOnly () const { return false; };

private:
  ElementProperty( const string& name, Accessor accessor, int position  ): FloatProperty( name ),
									   _accessor( accessor ), 
									   _position( position )
  {
  }

  Accessor _accessor;
  int _position;

};

class CameraConstructor
{
public:
  CameraConstructor(): _typeName( "LocalCamera" )
  {
  }

  void setup( const BaseCameraPtr& camera )
  {
    if( dynamic_cast< LocalCamera* >( camera.get() ) )
      _typeName = "LocalCamera";
    else if( dynamic_cast< WorldCamera* >( camera.get() ) )
      _typeName = "WorldCamera";
    else _typeName = "";
  }

  void construct( BaseCameraPtr& camera )
  {
    Transform3f transform = camera->transform();
    int32_t view = camera->guide()->view();
    Eigen::Vector3f up = camera->guide()->upAxis();

    if( _typeName == "LocalCamera" )
      camera.reset( new LocalCamera() );
    else if( _typeName == "WorldCamera" )
      camera.reset( new WorldCamera() );

    camera->setTransform( transform );
    camera->guide()->view() = view;
    camera->guide()->upAxis() = up;
  }

  PropertyPtr propertizeConstructor() { return buildProperty( "typeName", _typeName ); };
  ListPropertyPtr propertize( BaseCameraPtr& camera )
  { 
    ListPropertyPtr position = buildListProperty( "position",
					      buildPositionAccessor( "x", camera, 0 ),
					      buildPositionAccessor( "y", camera, 1 ),
					      buildPositionAccessor( "z", camera, 2 ) );

    PropertyPtr guideProperty = buildListProperty( "guide",
						   buildProperty( "view", (int32_t&)camera->guide()->view() ),
						   buildProperty( "up", camera->guide()->upAxis() ),
						   SimpleVectorProperty< Vector3f, 
									 VectorConstructor< Vector3f >, 
									 false >::create(
									   "points", 
									   "point", 
									   camera->guide()->points() ) );

    LocalCamera* local = dynamic_cast< LocalCamera* >( camera.get() );
    if( local != NULL )
    {
      return buildListProperty( "camera",
				guideProperty,
                                position,
                                buildListProperty( "rotation",
                                                   buildRotationAccessor( "angle", local, -1 ),
                                                   buildRotationAccessor( "x", local, 0 ),
                                                   buildRotationAccessor( "y", local, 1 ),
                                                   buildRotationAccessor( "z", local, 2 ) ) );
    }

    WorldCamera* world = dynamic_cast< WorldCamera* >( camera.get() );
    if( world != NULL )
    {
      return buildListProperty( "camera",
				guideProperty,
				position,
				buildListProperty( "rotation",
						   buildRotationAccessor( "x", world, 0 ),
						   buildRotationAccessor( "y", world, 1 ),
                                                   buildRotationAccessor( "z", world, 2 ) ) );
    }

    return buildListProperty( "camera" );
  };

private:
  class PositionAccessor
  {
  public:
    PositionAccessor( BaseCamera& camera ): _camera( camera ) {};

    float get( int position ) const { return _camera.position()( position ); };
    void set( float value, int axis ) 
    { 
      Vector3f position( _camera.position() );
      position( axis ) = value;

      _camera.setPosition( position );
    }

  private:
    BaseCamera& _camera;

  };

  class WorldRotationAccessor
  {
  public:
    WorldRotationAccessor( WorldCamera& camera ): _camera( camera ) {};

    float get( int position ) const { return _camera.rotation()( position ); };
    void set( float value, int axis ) 
    { 
      Vector3f rotation( _camera.rotation() );
      rotation( axis ) = value;

      _camera.setRotation( rotation );
    }

  private:
    WorldCamera& _camera;

  };

  class LocalRotationAccessor
  {
  public:
    LocalRotationAccessor( LocalCamera& camera ): _camera( camera ) {};

    float get( int position ) const 
    { 
      if( position == -1 ) 
	return _camera.rotation().angle();

      return _camera.rotation().axis()( position ); 
    }

    void set( float value, int axis ) 
    { 
      AngleAxisf rotation( _camera.rotation() );

      if( axis == -1 )
	rotation.angle() = value;
      else 
	rotation.axis()( axis ) = value;

      _camera.setRotation( rotation );
    }

  private:
    LocalCamera& _camera;

  };

  PropertyPtr buildPositionAccessor( const std::string& name, const BaseCameraPtr& camera, int position )
  {
    PositionAccessor accessor( *camera );
    return ElementProperty< PositionAccessor >::create( name, accessor, position );
  }

  PropertyPtr buildRotationAccessor( const std::string& name, LocalCamera* camera, int position )
  {
    LocalRotationAccessor accessor( *camera );
    return ElementProperty< LocalRotationAccessor >::create( name, accessor, position );
  }

  PropertyPtr buildRotationAccessor( const std::string& name, WorldCamera* camera, int position )
  {
    WorldRotationAccessor accessor( *camera );
    return ElementProperty< WorldRotationAccessor >::create( name, accessor, position );
  }

  std::string _typeName;

};

ListPropertyPtr Camera::properties()
{
  return buildListProperty( "camera", 
			    buildConstructorProperty< BaseCameraPtr, CameraConstructor >( "camera", _baseCamera ) );
}
