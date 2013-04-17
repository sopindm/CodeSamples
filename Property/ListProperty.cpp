/*
   -----------------------------------------------------------------------------
   |                 R T R T    P R O P E R T Y   L I B R A R Y                 |
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

#include <boost/foreach.hpp>
#include "PropertyVisitor.hpp"
#include "ListProperty.hpp"
#include "property_uuid.uuid.h"

using std::string;
using std::vector;
using namespace rtrt::property;

ListProperty::ListProperty( const string& name, const vector< PropertyPtr >& props ):
  Property( name ),
  _properties( props )
{

}

ListPropertyPtr ListProperty::create( const string& name, const vector< PropertyPtr >& props )
{
  return doCreate( new ListProperty( name, props ));
}

ListPropertyPtr ListProperty::doCreate( ListProperty* property )
{
  ListPropertyPtr ret( property );

  BOOST_FOREACH( PropertyPtr property, ret->_properties )
    property->setParent( *ret );

  ret->setSelfLink( ret );

  return ret;
}

uint32_t ListProperty::size() const
{
  return _properties.size();
}

const PropertyPtr& ListProperty::at( uint32_t index )
{
  assert( index >= 0 && index < _properties.size() );

  PropertyPtr& ret( _properties.at( index ));
  assert( ret != NULL );

  return ret;
}

uint32_t ListProperty::position( const string& name )
{
  uint32_t position( _properties.size() );

  for( size_t i=0; i<_properties.size(); i++ )
    if( _properties[ i ]->name() == name )
      position = i;

  return position;
}

const PropertyPtr& ListProperty::at( const string& name )
{
  size_t position( -1 );

  for( size_t i=0; i<_properties.size(); i++ )
    if( _properties[ i ]->name() == name )
      position = i;

  return at( position );
}

void ListProperty::accept( PropertyVisitor& visitor )
{
  visitor.visit( *this );
}

ListPropertyPtr ListProperty::append( const PropertyPtr& property ) const
{
  vector< PropertyPtr > newProps( _properties );
  newProps.push_back( property );

  return ListPropertyPtr( new ListProperty( name(), newProps ));
}

ListPropertyPtr ListProperty::append( const PropertyPtr& property1,
				      const PropertyPtr& property2 ) const
{
  return append( property1 )->append( property2 );
}

ListPropertyPtr ColorProperty::create( const string& name, const vector< PropertyPtr >& props )
{
  return ListProperty::doCreate( new ColorProperty( name, props ));
}

const unsigned char* ListProperty::type_uuid() const
{
  return list_property_uuid;
}
