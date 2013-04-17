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

#include "PropertyWriter.hpp"

using namespace rtrt::property;
using namespace rtrt::convert;

PropertyWriter::PropertyWriter( const SceneOStreamPtr& stream ): _stream( stream )
{
}

void PropertyWriter::setStream( const SceneOStreamPtr& stream )
{
  _stream = stream;
}

void PropertyWriter::write( const PropertyPtr& property )
{
  property->accept( *this );
}

void PropertyWriter::visit( property::BoolProperty& property )
{
  doVisit( property );
}

void PropertyWriter::visit( property::IntProperty& property )
{
  doVisit( property );
}

void PropertyWriter::visit( property::UIntProperty& property )
{
  doVisit( property );
}

void PropertyWriter::visit( property::FloatProperty& property )
{
  doVisit( property );
}

void PropertyWriter::visit( property::StringProperty& property )
{
  doVisit( property );
}

class PropertyWriter::ElementWriter : public PropertyVisitor
{
public:
  ElementWriter( PropertyWriter& writer ): _writer( writer ) {};

  void visit( BoolProperty& property ) { return _writer.visitElement( property ); };
  void visit( IntProperty& property ) { return _writer.visitElement( property ); };
  void visit( UIntProperty& property ) { return _writer.visitElement( property ); };
  void visit( FloatProperty& property ) { return _writer.visitElement( property ); };
  void visit( StringProperty& property ) { return _writer.visitElement( property ); };

  void visit( ListProperty& property ){ return _writer.visit( property ); };
  void visit( VectorProperty& property ){ return _writer.visit( property ); };

  void visit( ConstructorProperty& property ) { return _writer.visit( property ); };

private:
  PropertyWriter& _writer;

};

void PropertyWriter::visit( property::VectorProperty& property )
{
  _stream->enterSection( property.name() );

  _stream->write( property.size(), "size" );

  void* rawData( property.rawData() );

  if( rawData != NULL && _stream->writeRawData( rawData, property.size() * property.elementSize() ) )
  {
  }
  else
  {
    ElementWriter writer( *this );
    for( size_t i=0; i<property.size(); i++ )
      property.at( i )->accept( writer );
  }

  _stream->leaveSection();
}

void PropertyWriter::visit( property::ListProperty& property )
{
  _stream->enterSection( property.name() );

  for( size_t i=0; i<property.size(); i++ )
    write( property.at( i ) );

  _stream->leaveSection();
}

void PropertyWriter::visit( property::ConstructorProperty& property )
{
  _stream->enterSection( property.name() );

  write( property.constructor() );

  ListPropertyPtr constructed( property.constructed() );
  for( size_t i=0; i<constructed->size(); i++ )
    write( constructed->at( i ) );

  _stream->leaveSection();
}

template< class T >
void PropertyWriter::visitElement( TypedProperty< T >& property )
{
  _stream->enterSection( property.name() );
  
  doVisit( property, "value" );

  _stream->leaveSection();
}

template< class T >
void PropertyWriter::doVisit( property::TypedProperty< T >& property )
{
  doVisit( property, property.name() );
}

template< class T >
void PropertyWriter::doVisit( property::TypedProperty< T >& property, const std::string& name )
{
  _stream->write( property.get(), name );
}
