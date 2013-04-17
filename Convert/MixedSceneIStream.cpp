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

#include <boost/lexical_cast.hpp>
#include "MixedSceneIStream.hpp"

using std::istream;
using std::tr1::shared_ptr;
using namespace rtrt::convert;

MixedSceneIStream::MixedSceneIStream( shared_ptr< istream > baseStream,
				      shared_ptr< istream > binaryStream ):
				      XmlSceneIStream( XmlNodePtr() ),
				      _baseStream( baseStream ),
				      _tableStream( new TableSceneIStream( binaryStream )),
				      _checking( false ),
				      _checkLog()
{
}

void MixedSceneIStream::init()
{
  _tableStream->initOffsets();
  setRoot( XmlNode::parse( *_baseStream ) );
}

void MixedSceneIStream::checkSha()
{
  uint32_t id( current()->attr( "id" ).as< uint32_t >() );
  std::string sha( current()->attr( "sha" ).as< std::string >() );
  std::string actualSha( _tableStream->sha( id ) );
  
  if( sha != actualSha )
  {
    pushError( std::string( "Wrong sha of node " ) + boost::lexical_cast< std::string >( id )
	       + " Actual: " + actualSha + " Expected: " + sha + '\n' );
  }
}

bool MixedSceneIStream::readRawData( void* data, size_t size )
{
  _tableStream->beginNode( current()->attr( "id" ).as< uint32_t >() );
  bool result = _tableStream->baseStream()->readRawData( data, size );

  assert( result );

  if( _checking )
    checkSha();

  return true;
}

void MixedSceneIStream::setChecking( bool check )
{
  _checking = check;
}

const std::string& MixedSceneIStream::checkLog() const
{
  return _checkLog;
}
