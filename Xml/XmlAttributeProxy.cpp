/*
   -----------------------------------------------------------------------------
   |                 R T R T    X M L   L I B R A R Y                 |
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

#include "XmlAttributeProxy.hpp"

using std::ostream;
using std::string;
using namespace rtrt;

XmlAttributeProxy::XmlAttributeProxy( const string& name, AttributeMap::iterator position, AttributeMap& map ): _name( name ), _position( position ), _map( map )
{
}

void XmlAttributeProxy::operator=( const XmlAttribute& value )
{
  _map[ _name ] = value;
}

void XmlAttributeProxy::operator=( const XmlAttribute::ValueType& value )
{
  _map[ _name ] = value;
}

bool XmlAttributeProxy::operator==( const XmlAttribute::ValueType& value ) const
{
  if( _position == _map.end() )
    return false;
  return _position->second == value;
}

bool XmlAttributeProxy::operator!=( const XmlAttribute::ValueType& value ) const
{
  return !( *this == value ); 
}

bool XmlAttributeProxy::isSet() const
{
  return _position != _map.end();
}

const XmlAttribute::ValueType& XmlAttributeProxy::value() const
{
  return _position->second.value();
}

ostream& rtrt::operator<<( ostream& stream, const XmlAttributeProxy& tag )
{
  if( tag._position == tag._map.end() ) 
    stream << "";
  else
    stream << tag._position->second.as< string >();
  
  return stream;
}
