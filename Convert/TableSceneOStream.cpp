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

#include <openssl/sha.h>
#include "BinarySceneOStream.hpp"
#include "TableSceneOStream.hpp"

using std::string;
using std::stringstream;
using std::ostream;
using std::tr1::shared_ptr;
using std::tr1::static_pointer_cast;
using namespace rtrt::convert;

TableSceneOStream::TableSceneOStream( const shared_ptr< ostream >& stream ) : _stream( stream ),
                                                                              _tempStream( new stringstream ),
                                                                              _binaryStream( new BinarySceneOStream( _tempStream ))
{
}

SceneOStreamPtr TableSceneOStream::baseStream()
{
  return _binaryStream;
}

uint32_t TableSceneOStream::beginNode()
{
  _offsets.push_back( _tempStream->tellp() );
  return _offsets.size() - 1;
}

string TableSceneOStream::endNode()
{
  uint32_t offset = _tempStream->tellp();
  uint32_t length = offset - _offsets[ _offsets.size() - 1 ];
  char* data = new char[ length ];
  _tempStream->seekg( _offsets[ _offsets.size() - 1 ] );
  
  _tempStream->read( data, length );
  _tempStream->seekg( 0 );
  
  unsigned char sha[ 20 ];
  SHA1( reinterpret_cast< unsigned char* >( data ), length, sha );
  delete[] data;
  
  string ret;
  for( int i=0; i<20; i++ )
  {
    char higher( sha[ i ]/16 );
    char lower( sha[ i ]%16 );
    
    if( higher < 10 )
      ret += '0' + higher;
    else
      ret += 'a' + higher - 10;
    
    if( lower < 10 )
      ret += '0' + lower;
    else 
      ret += 'a' + lower - 10;
  }
  return ret;
}

void TableSceneOStream::writeSize( const uint32_t& size )
{
  const char* data = reinterpret_cast< const char* >( &size );
  _stream->write( data, sizeof( uint32_t ) );
}

void TableSceneOStream::flush()
{
  writeSize( _offsets.size() );
  for( uint32_t i=0; i<_offsets.size(); i++ )
    writeSize( _offsets[ i ]);
  *_stream << _tempStream->rdbuf();
}
