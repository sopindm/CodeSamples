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

#include "PropertyReader.hpp"

using namespace rtrt::property;
using namespace rtrt::convert;

PropertyReader::PropertyReader( const SceneIStreamPtr& stream ): _stream( stream )
{
}

void PropertyReader::setStream( const SceneIStreamPtr& stream )
{
  _stream = stream;
}

void PropertyReader::read( const PropertyPtr& property )
{
  property->accept( *this );
}

void PropertyReader::visit( property::BoolProperty& property )
{
  doVisit( property );
}

void PropertyReader::visit( property::IntProperty& property )
{
  doVisit( property );
}

void PropertyReader::visit( property::UIntProperty& property )
{
  doVisit( property );
}

void PropertyReader::visit( property::FloatProperty& property )
{
  doVisit( property );
}

void PropertyReader::visit( property::StringProperty& property )
{
  doVisit( property );
}

class PropertyReader::ElementReader : public PropertyVisitor
{
public:
  ElementReader( PropertyReader& reader ): _reader( reader ) {};

  void visit( BoolProperty& property ) { return _reader.visitElement( property ); };
  void visit( IntProperty& property ) { return _reader.visitElement( property ); };
  void visit( UIntProperty& property ) { return _reader.visitElement( property ); };
  void visit( FloatProperty& property ) { return _reader.visitElement( property ); };
  void visit( StringProperty& property ) { return _reader.visitElement( property ); };

  void visit( ListProperty& property ){ return _reader.visit( property ); };
  void visit( VectorProperty& property ){ return _reader.visit( property ); };

  void visit( ConstructorProperty& property ) { return _reader.visit( property ); };

private:
  PropertyReader& _reader;

};

void PropertyReader::visit( property::VectorProperty& property )
{
  _stream->enterSection( property.name() );

  uint32_t size;
  _stream->read( &size, "size" );

  property.resize( size );

  void *rawData( property.rawData() );

  if( rawData != NULL && _stream->readRawData( rawData, size * property.elementSize() ) )
  {
  }
  else
  {
    ElementReader reader( *this );
    for( size_t i=0; i<size; i++ )
      property.at( i )->accept( reader );
  }

  _stream->leaveSection();
}

void PropertyReader::visit( ListProperty& property )
{
  _stream->enterSection( property.name() );

  for( size_t i=0; i<property.size(); i++ )
    read( property.at( i ));

  _stream->leaveSection();
}

void PropertyReader::visit( ConstructorProperty& property )
{
  _stream->enterSection( property.name() );

  read( property.constructor() );

  ListPropertyPtr constructed( property.constructed() );
  for( size_t i=0; i<constructed->size(); i++ )
    read( constructed->at( i ) );

  _stream->leaveSection();
}

template< class T >
void PropertyReader::visitElement( TypedProperty< T >& property )
{
  _stream->enterSection( property.name() );

  doVisit( property, "value" );

  _stream->leaveSection();
}

template< class T >
void PropertyReader::doVisit( TypedProperty< T >& property )
{
  doVisit( property, property.name() );
}

template< class T >
void PropertyReader::doVisit( TypedProperty< T >& property, const std::string& name )
{
  T value;
  _stream->read( &value, name );

  property.set( value );
}

namespace rtrt
{
  namespace convert
  {
    template<>
    void rtrt::convert::PropertyReader::doVisit< bool >( BoolProperty& property, const std::string& name )
    {
      int value;
      _stream->read( &value, name );

      property.set( value != 0 );
    }
  }
}
