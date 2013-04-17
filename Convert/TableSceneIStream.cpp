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
#include "BinarySceneIStream.hpp"
#include "TableSceneIStream.hpp"

using std::istream;
using std::tr1::shared_ptr;
using namespace rtrt::convert;

TableSceneIStream::TableSceneIStream( const shared_ptr< istream >& stream ):
                                      _stream( stream ),
                                      _binaryStream( new BinarySceneIStream( stream ))
{
}

void TableSceneIStream::initOffsets()
{
  uint32_t size( readSize() );
  _offsets.resize( size );
  for( uint32_t i=0; i<_offsets.size(); i++ )
    _offsets[ i ] = readSize();
}

uint32_t TableSceneIStream::readSize()
{
  uint32_t ret;
  _stream->read( reinterpret_cast< char* >( &ret ), sizeof( uint32_t ) );
  
  return ret;
}

SceneIStreamPtr TableSceneIStream::baseStream()
{
  return _binaryStream;
}

void TableSceneIStream::beginNode( uint32_t index )
{
  assert( index>=0 && index<_offsets.size() ); 
  
  uint32_t offsetsLength = ( _offsets.size() + 1 ) * sizeof( uint32_t );
  _stream->seekg( offsetsLength + _offsets[ index ] );
}

std::string TableSceneIStream::sha( uint32_t index )
{
  assert( index>=0 && index<_offsets.size() );
  
  uint32_t size;
  if( index < _offsets.size()-1 )
    size = _offsets[ index+1 ] - _offsets[ index ];
  else 
    size = totalSize() - _offsets[ index ];
    
  beginNode( index );
  char* buffer( new char[ size ] );
  _stream->read( buffer, size );
  
  std::string ret( countSha( buffer, size ) );
  
  delete[] buffer;
  return ret;
}

uint32_t TableSceneIStream::totalSize()
{
  uint32_t endOffset = _stream->tellg();
  return endOffset - ( _offsets.size()+1 ) * sizeof( uint32_t );
}

std::string TableSceneIStream::countSha( char* buffer, uint32_t size )
{
  unsigned char sha[ 20 ];
  SHA1( reinterpret_cast< unsigned char* >( buffer ), size, sha );
  
  std::string ret;
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
