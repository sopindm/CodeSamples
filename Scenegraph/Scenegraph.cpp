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
#include "ModelStash.hpp"
#include "StashFactory.hpp"
#include "Scenegraph.hpp"
#include "Lights/EnvironmentMap.hpp"

using namespace boost;
using std::string;
using namespace rtrt::scenegraph;

Scenegraph::Scenegraph( Node* root ): _root( root ),
				      _time( Time::lowest ),
				      _updateTime( -INFINITY ),
				      _updateDelta( 1e-2 )
{
}

const Time& Scenegraph::time() const
{
  return _time;
}

Node* Scenegraph::root () const
{
  return _root;
}

void Scenegraph::update( double time )
{
  _time++;

  if( fabs( time - _updateTime ) > _updateDelta )
  {
    updateNode( _root, time, AttributeList() );
    _updateTime = time;
  }
}

void Scenegraph::updateNode( Node* node, double time, const AttributeList& baseAttributes )
{
  Kernel* kernel( node->kernel().get() );
  AttributeList oldAttributes( node->attributes() );

  node->attributes() = AttributeList();

  if( kernel != NULL )
  {
    kernel->assignNode( node );
    kernel->update( time );

    if( kernel->_time == Time( Time::newest ))
      kernel->_time = _time;

    BaseStashable* stashable = dynamic_cast< BaseStashable* >( node->kernel().get() );
    if( stashable != NULL )
    {
      BOOST_FOREACH( string name, stashable->stashes() )
      {
	ensureHaveStash( name );

	BaseStashPtr stash( _stashes[ name ] );

	stash->writeObject( node, baseAttributes, _time );
	stash->interface( stashable )->resolveNewest( _time );
      }
    }
  }

  AttributeList attributes = baseAttributes.merge( node->attributes() );

  AttributeList old( oldAttributes.delta( node->attributes() ) );
  attributes.touch( old );

  for( size_t i=0; i<node->childs().size(); i++ )
    updateNode( node->childs()[ i ].get(), time, attributes );
}

void Scenegraph::addNode( Node* node )
{
  std::string typen = node->kernel()->typeName();

  node->kernel()->assignNode( node );

  BaseStashable* stashable = dynamic_cast< BaseStashable* >( node->kernel().get() );
  if( stashable != NULL )
  {
    BOOST_FOREACH( string name, stashable->stashes() )
    {
      ensureHaveStash( name );
      
      BaseStashPtr currStash( _stashes[ name ] );

      currStash->addObject( node );
      currStash->allocateObject( node );
    }
  }
}

void Scenegraph::ensureHaveStash( const std::string& name )
{
  StashMap::iterator stash( _stashes.find( name ));

  if( stash == _stashes.end() )
  {
    BaseStash* newStash( StashFactory::create( name ));
    assert( newStash != NULL );

    BaseStashPtr newStashPtr( newStash );

    _stashes[ name ] = newStashPtr;
    
    GeometryStashPtr geometry = dynamic_pointer_cast< GeometryStash >( newStashPtr );
    if( geometry != NULL )
    {
      newStash->scenegraphID().geometryID = _geometry.size();
      _geometry.push_back( geometry );
    }

    MaterialStashPtr material = dynamic_pointer_cast< BaseMaterialStash >( newStashPtr );
    if( material != NULL )
    {
      newStash->scenegraphID().materialID = _materials.size();
      _materials.push_back( material );
    }

    LightStashPtr light = dynamic_pointer_cast< LightStash >( newStashPtr );
    if( light != NULL )
    {
      newStash->scenegraphID().lightID = _lights.size();
      _lights.push_back( light );
    }
  }
}

void Scenegraph::removeNode( Node* node )
{
  if( node->kernel() != NULL )
    node->kernel()->assignNode( node );

  BaseStashable* stashable = dynamic_cast< BaseStashable* >( node->kernel().get() );
  
  if( stashable != NULL )
  {
    BOOST_FOREACH( string name, stashable->stashes() )
    {
      BaseStashPtr stash( _stashes[ name ] );

      stash->freeObject( node );
      stash->removeObject( node );
    }
  }
}

BaseStashPtr Scenegraph::stash( const string& name ) const
{
  StashMap::const_iterator it( _stashes.find( name ));

  if( it == _stashes.end() )
    return BaseStashPtr();

  return it->second;
}

const GeometryVector& Scenegraph::geometry() const
{
  return _geometry;
}

const MaterialStashVector& Scenegraph::materials() const
{
  return _materials;
}

const LightVector& Scenegraph::lights() const
{
  return _lights;
}

bool Scenegraph::haveEnvironmentMap() const
{
  StashMap::const_iterator it( _stashes.find( EnvironmentMapStash::stashName() ));

  if( it == _stashes.end() )
    return false;

  EnvironmentMapStash* eStash = dynamic_cast< EnvironmentMapStash* >( it->second.get() );

  if( eStash != NULL && eStash->lights() == 1 )
    return true;

  return false;
}

size_t Scenegraph::triangles() const
{
  size_t ret( 0 );

  BOOST_FOREACH( GeometryStashPtr stash, _geometry )
  {
    ModelStash* mStash = dynamic_cast< ModelStash* >( stash.get() );

    if( mStash != NULL )
      ret += mStash->indices().size();
  }

  return ret;
}
