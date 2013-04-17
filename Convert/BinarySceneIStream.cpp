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

#include "BinarySceneIStream.hpp"

using std::string;
using namespace Eigen;
using namespace rtrt::convert;

BinarySceneIStream::BinarySceneIStream( std::tr1::shared_ptr< std::istream > stream ) : _stream( stream ), _indentation( 0 )
{
}
    
void BinarySceneIStream::read( int* value, const string& )
{
  _stream->read( reinterpret_cast< char* >( value ), sizeof( int ));
}

void BinarySceneIStream::read( uint32_t* value, const string& )
{
  _stream->read( reinterpret_cast< char* >( value ), sizeof( uint32_t ));
}

void BinarySceneIStream::read( float* value, const string& )
{
  _stream->read( reinterpret_cast< char* >( value ), sizeof( float ));
}

void BinarySceneIStream::read( std::string* str, const string& )
{
  uint32_t size;
  read( &size, "size" );
  
  char* buffer = new char[ size ];
  _stream->read( buffer, size );
  
  str->assign( buffer, size );
  delete[] buffer;
}

bool BinarySceneIStream::readRawData( void* data, size_t size )
{
  _stream->read( reinterpret_cast< char* >( data ), size );
  return true;
}
    
void BinarySceneIStream::enterSection( const string& )
{
  _indentation++;
}

void BinarySceneIStream::leaveSection()
{
  _indentation--;
}
    
bool BinarySceneIStream::correct()
{
  bool isCorrect = true;
  
  if( _stream->bad() || _stream->fail() )
  {
    isCorrect = false;
    pushError( "Base stream bad." );
  }
  if( _indentation != 0 )
  {
    isCorrect = false;
    pushError( "Section's end missing." );
  }
  
  return isCorrect;
}
