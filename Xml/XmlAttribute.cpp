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

#include <string>
#include "XmlAttribute.hpp"

using std::string;
using std::ostream;
using namespace rtrt;

XmlAttribute::XmlAttribute() : _value()
{
}

void XmlAttribute::operator=( const ValueType& value )
{
  _value = value;
}

bool XmlAttribute::operator==( const ValueType& value ) const
{
  return _value == value;
}

class XmlAttribute::ToString : public boost::static_visitor< string >
{
public:
  string operator()( const string& value ) const
  {
    return "\"" + value + "\"";
  }
  string operator()( const Decimal& value ) const
  {
    return value.as< string >();
  }
};

string XmlAttribute::toString() const
{
  return boost::apply_visitor( XmlAttribute::ToString(), _value );
}
 
ostream& rtrt::operator<<( ostream& stream, const XmlAttribute& attr )
{
  string value = boost::apply_visitor( XmlAttribute::ToString(), attr._value );
  
  stream << value;
  return stream;
}
