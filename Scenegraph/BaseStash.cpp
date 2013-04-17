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

#include <boost/foreach.hpp>
#include "scenegraph_uuid.uuid.h"
#include "BaseStash.hpp"

using namespace rtrt::scenegraph;

BaseStash::BaseStash(): _ids()
{
}

BaseStashable* BaseStash::nodeInterface( Node* object )
{
  assert( object != NULL );

  BaseStashable* stashable = dynamic_cast< BaseStashable* >( object->kernel().get() );

  assert( stashable != NULL );

  return stashable;
}

void BaseStash::addObject( Node* object )
{
  addObject( object, nodeInterface( object ) );
}

void BaseStash::addObject( Node* node, BaseStashable* stashable )
{
  size_t id;

  if( _ids.count( node ) == 0 )
  {
    size_t size = _ids.size();

    _ids[ node ] = id = size;
  }
  else
  {
    id = _ids[ node ];
  }

  interface( stashable )->addToStash( *this );

  doAddObject( stashable, id );
}

void BaseStash::removeObject( Node* node )
{
  removeObject( node, nodeInterface( node ) );
}

void BaseStash::removeObject( Node* node, BaseStashable* stashable )
{
  StashMap::iterator iterator( _ids.find( node ) );

  if( iterator == _ids.end() )
    return;

  _ids.erase( iterator );

  size_t id = iterator->second;

  interface( stashable )->removeFromStash();

  doRemoveObject( stashable, id );

  std::pair< StashMap::key_type, StashMap::mapped_type > object;

  BOOST_FOREACH( object, _ids )
  {
    if( object.second > id )
      _ids[ object.first ]--;
  }
}

void BaseStash::allocateObject( Node* node )
{
  allocateObject( node, nodeInterface( node ) );
}

void BaseStash::allocateObject( Node* node, BaseStashable* stashable )
{
  StashMap::iterator iterator( _ids.find( node ) );

  assert( iterator != _ids.end() );

  size_t id = iterator->second;
  
  doAllocateObject( stashable, id );

  interface( stashable )->setNew();

  setNew();
}

void BaseStash::freeObject( Node* node )
{
  freeObject( node, nodeInterface( node ) );
}

void BaseStash::freeObject( Node* node, BaseStashable* stashable )
{
  StashMap::iterator iterator( _ids.find( node ) );

  assert( iterator != _ids.end() );

  size_t id = iterator->second;

  doFreeObject( stashable, id );

  setNew();
}

void BaseStash::writeObject( Node* node, const AttributeList& list, const Time& time )
{
  writeObject( node, nodeInterface( node ), list, time );
}

#include <glog/logging.h>

void BaseStash::writeObject( Node* node, BaseStashable* stashable, const AttributeList& list, const Time& time )
{
  StashMap::iterator iterator( _ids.find( node ) );

  if( iterator == _ids.end() )
    return;

  size_t id( iterator->second );

  doWriteObject( stashable, id, list, time );
}

size_t BaseStash::id( Node* node )
{
  StashMap::iterator iterator( _ids.find( node ) );

  if( iterator == _ids.end() )
    return -1;

  return iterator->second;
}

BaseStash::ScenegraphID& BaseStash::scenegraphID()
{
  return _scenegraphID;
}

const unsigned char* BaseStash::type_uuid() const
{
  return stash_uuid;
}

BaseStashInterface::BaseStashInterface(): _propertyHandler( new BasePropertyHandler( this ) ), _stash( NULL )
{
}

void BaseStashInterface::addToStash( BaseStash& stash )
{
  _stash = &stash;
}

void BaseStashInterface::removeFromStash()
{
}

void BaseStashInterface::allocate()
{
  assert( _stash != NULL );
  _stash->allocateObject( node() );
}

void BaseStashInterface::free()
{
  assert( _stash != NULL );
  _stash->freeObject( node() );
}

void BaseStashInterface::setNew()
{
  Timed::setNew();
  renew();
}









