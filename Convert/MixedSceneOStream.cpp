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

#include "MixedSceneOStream.hpp"

using std::string;
using std::ostream;
using std::tr1::shared_ptr;

using namespace rtrt::convert;

MixedSceneOStream::MixedSceneOStream( shared_ptr< ostream > baseStream,
				      shared_ptr< ostream > binaryStream ):
				      XmlSceneOStream( XmlNode::create( "xml" )),
				      _tableStream( new TableSceneOStream( binaryStream )),
				      _baseStream( baseStream )
{
}

void MixedSceneOStream::flush()
{
  root()->write( *_baseStream );
  _tableStream->flush();
}

bool MixedSceneOStream::writeRawData( void* data, size_t size )
{
  int node( _tableStream->beginNode() );
  _tableStream->baseStream()->writeRawData( data, size );

  string sha( _tableStream->endNode() );
  
  XmlSceneOStream::write( node, "id" );
  XmlSceneOStream::write( sha, "sha" );

  return true;
}

