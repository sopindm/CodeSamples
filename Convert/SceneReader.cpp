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
#include <Scenegraph/ObjectFactory.hpp>
#include "PropertyReader.hpp"
#include "SceneReader.hpp"

using std::string;
using namespace rtrt::property;
using namespace rtrt::scenegraph;
using namespace rtrt::convert;

SceneReader::SceneReader( SceneIStreamPtr stream ): _stream( stream ),
						    _ids()
{
}

NodePtr SceneReader::read()
{
  _stream->enterSection( "scene" );

  string version;
  _stream->read( &version, "version" );

  if( version != "0.1" )
  {
    LOG( ERROR ) << "Unknown file version";
    exit( EXIT_FAILURE );
  }

  _stream->enterSection( "kernels" );

  uint32_t size;
  _stream->read( &size, "size" );

  for( uint32_t i=0; i<size; i++ )
    readKernel();

  _stream->leaveSection();
  
  NodePtr scene( Node::create() );
  readNode( scene );

  _stream->leaveSection();
  _ids.clear();

  return scene;
}

void SceneReader::readKernel()
{
  _stream->enterSection( "kernel" );

  uint32_t id;
  _stream->read( &id, "id" );

  string type;
  _stream->read( &type, "type" );

  KernelPtr newKernel = ObjectFactory::createKernel( type );
  if( newKernel == NULL )
  {
    LOG( ERROR ) << "Unknown kernel type " << type;
    exit( EXIT_FAILURE );
  }

  _ids[ id ] = newKernel;

  ListPropertyPtr property( _ids[ id ]->properties() );
  PropertyReader reader( _stream );

  for( size_t i=0; i<property->size(); i++ )
    reader.read( property->at( i ) );

  _stream->leaveSection();
}

void SceneReader::readNode( NodePtr& node )
{
  _stream->enterSection( "node" );

  string name;
  _stream->read( &name, "name" );

  node->setName( name );

  uint32_t childs;
  _stream->read( &childs, "childs" );

  uint32_t kernelId;
  _stream->read( &kernelId, "kernelID" );

  if( kernelId < _ids.size() )
    node->setKernel( kernelById( kernelId ));

  for( uint32_t i=0; i<childs; i++ )
  {
    NodePtr child( Node::create() );
    readNode( child );

    node->childs() += child;
  }

  _stream->leaveSection();
}

const KernelPtr& SceneReader::kernelById( uint32_t id )
{
  KernelIds::iterator iter( _ids.find( id ));

  if( iter == _ids.end() )
  {
    LOG( ERROR ) << "Unknown kernel id " << id;
    exit( EXIT_FAILURE );
  }

  return iter->second;
}
