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

#include <glog/logging.h>
#include <boost/foreach.hpp>
#include <Core/VectorUtils.hpp>
#include "scenegraph_uuid.uuid.h"
#include "CameraStash.hpp"

using namespace boost;
using namespace Eigen;
using namespace rtrt;
using namespace rtrt::scenegraph;
using namespace rtrt::scenegraph::cameraStash;

StashInterface::StashInterface(): _baseTransform( VectorUtils::IdentityTransform() ),
				  _viewport( Viewport::create() )
{
}

const Transform3f& StashInterface::baseTransform() const
{
  return _baseTransform;
}

void StashInterface::setBaseTransform( const Eigen::Transform3f& transform )
{
  _baseTransform = transform;
}

const ViewportPtr& StashInterface::viewport() const
{
  return _viewport;
}

void StashInterface::setViewport( const ViewportPtr& viewport )
{
  _viewport = viewport;
}

CameraStash::CameraStash()
{
}

const CameraArray& CameraStash::cameras() const
{
  return _cameras.value();
}

uint32_t CameraStash::cameraID( const ScenegraphCameraPtr& camera ) const
{
  for( uint32_t i=0; i<_cameras.value().size(); i++ )
    if( _cameras.value()[ i ] == camera )
      return i;

  return _cameras.value().size();
}

void CameraStash::doAddObject( BaseStashable*, size_t id )
{
  _cameras.insertIndex( id );
}

void CameraStash::doRemoveObject( BaseStashable*, size_t id )
{
  _cameras.eraseIndex( id );
}

void CameraStash::doAllocateObject( BaseStashable* object, size_t id )
{
  _cameras.allocate( id, 1 );
}

void CameraStash::doFreeObject( BaseStashable* object, size_t id )
{
  _cameras.free( id );
}

void CameraStash::doWriteObject( BaseStashable* object, size_t id, const AttributeList& attrs, const Time& time )
{
  StashInterface* camera( interfaceCast< StashInterface >( object ));
  camera->stash( _cameras.at( id ) );

  TransformAttributePtr transform( SCENEGRAPH_ATTRIBUTE( Transform, attrs ));

  if( transform->isNew( time ) || camera->isNew( time ) )
  {
    setNew();
    camera->setBaseTransform( transform->value() );
  }
}

const unsigned char* CameraStash::type_uuid() const
{
  return camera_stash_uuid;
}
