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

#include "BinarySceneOStream.hpp"

using Eigen::Vector4i;
using namespace rtrt::convert;

BinarySceneOStream::BinarySceneOStream( std::tr1::shared_ptr< std::ostream > stream ) : _stream( stream ), _indentation( 0 )
{
}
    
void BinarySceneOStream::write( int value, const std::string& fieldName )
{
  _stream->write( reinterpret_cast<char*>( &value ), sizeof( int ));
}

void BinarySceneOStream::write( uint32_t value, const std::string& )
{
  _stream->write( reinterpret_cast<char*>( &value ), sizeof( uint32_t ));
}

void BinarySceneOStream::write( float value, const std::string& fieldName )
{
  _stream->write( reinterpret_cast<char*>( &value ), sizeof( float )); 
}

void BinarySceneOStream::write( const std::string& string, const std::string& )
{
  uint32_t size( string.size());
  write( size, "size" );
  
  _stream->write( string.data(), string.size());
}

bool BinarySceneOStream::writeRawData( void* data, size_t size )
{
  _stream->write( reinterpret_cast< const char* >( data ), size );
  return true;
}

void BinarySceneOStream::enterSection( const std::string& name )
{
  _indentation++;
}

void BinarySceneOStream::leaveSection()
{
  _indentation--;
}
    
bool BinarySceneOStream::correct()
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
