/*
   -----------------------------------------------------------------------------
   |                 R T R T    C O N V E R T   L I B R A R Y                 |
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
#include "PropertyWriter.hpp"
#include "SceneWriter.hpp"

using namespace rtrt::property;
using namespace rtrt::scenegraph;
using namespace rtrt::convert;

SceneWriter::SceneWriter( SceneOStreamPtr stream ): _stream( stream ),
						    _ids()
{
}

void SceneWriter::write( const NodePtr& node )
{
  _stream->enterSection( "scene" );
  _stream->write( "0.1", "version" );

  traverseNode( node );

  _stream->enterSection( "kernels" );

  _stream->write( static_cast< uint32_t >( _ids.size() ), "size" );

  BOOST_FOREACH( KernelId id, _ids )
    writeKernel( id.first, id.second );

  _stream->leaveSection();

  writeNode( node );

  _stream->leaveSection();
}

void SceneWriter::traverseNode( const NodePtr& node )
{
  assert( node != NULL );

  if( node->kernel() != NULL )
    kernelId( node->kernel().get(), true );

  for( size_t i=0; i<node->childs().size(); i++ )
    traverseNode( node->childs()[ i ] );
}

void SceneWriter::writeNode( const NodePtr& node )
{
  assert( node != NULL );

  _stream->enterSection( "node" );
  
  _stream->write( node->name(), "name" );
  _stream->write( static_cast< uint32_t >( node->childs().size() ), "childs" );

  if( node->kernel() != NULL )
    _stream->write( kernelId( node->kernel().get(), false ), "kernelID" );
  else
    _stream->write( static_cast< uint32_t >( -1 ), "kernelID" );

  for( size_t i=0; i<node->childs().size(); i++ )
    writeNode( node->childs()[ i ] );

  _stream->leaveSection();
}

void SceneWriter::writeKernel( Kernel* kernel, uint32_t id )
{
  _stream->enterSection( "kernel" );

  _stream->write( id, "id" );
  _stream->write( kernel->typeName(), "type" );

  ListPropertyPtr properties( kernel->properties() );

  PropertyWriter writer( _stream );
  for( size_t i=0; i<properties->size(); i++ )
    writer.write( properties->at( i ) );

  _stream->leaveSection();
}

uint32_t SceneWriter::kernelId( Kernel* kernel, bool addIfDoesNotExists )
{
  KernelIds::iterator index( _ids.find( kernel ));
  if( index == _ids.end() )
  {
    if( !addIfDoesNotExists )
    {
      LOG( ERROR ) << "Wrong kernel " << kernel;
      LOG( ERROR ) << "Scene writting failed.";

      exit( EXIT_FAILURE );
    }

    size_t newId = _ids.size();
    _ids[ kernel ] = newId;

    return newId;
  }

  return index->second;
}
