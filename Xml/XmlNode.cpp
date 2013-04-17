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

#include <boost/foreach.hpp>
#include "XmlNode.hpp"

using std::string;
using std::istream;
using std::ostream;
using namespace rtrt;

XmlNode::XmlNode( const string& name ): _name( name ), _parent( NULL )
{
}

XmlNodePtr XmlNode::create( const string& name )
{
  return XmlNodePtr( new XmlNode( name ));
}

string& XmlNode::name()
{
  return _name;
}

const string& XmlNode::name() const
{
  return _name;
}

void XmlNode::write( ostream& stream, int indentation ) const
{
  stream << makeOpeningTag( indentation );
 
  if( _childs.size() > 0 )
  {
    BOOST_FOREACH( XmlNodePtr node, childs() )
    {
      stream << std::endl;
      node->write( stream, indentation+1 );
    }
    stream << std::endl;
    stream << makeClosingTag( indentation );
  }
}

string XmlNode::makeIndentation( int indentation )
{
  string ret;
  for( int i=0; i<indentation; i++ )
    ret += "  ";
  
  return ret;
}

string XmlNode::makeOpeningTag( int indentation ) const
{
  string ret( makeIndentation( indentation ) );
  ret += "<"; 
  ret += _name;
  
  for( AttributeMap::const_iterator i = _attributes.begin(); i != _attributes.end(); i++ )
  {
    ret += ' ';
    ret += i->first;
    ret += '=';
    ret += i->second.toString();
  }
  
  if( _childs.size() == 0 )
    ret += "/>";
  else
    ret += ">";
  
  return ret;
}

string XmlNode::makeClosingTag( int indentation ) const
{
  string ret( makeIndentation( indentation ));
  ret += "</";
  ret += _name;
  ret += '>';
  
  return ret;
}

XmlAttributeProxy XmlNode::attr( const string& name )
{
  return XmlAttributeProxy( name, _attributes.find( name ), _attributes );
}

InsertableXmlNodesProxy XmlNode::childs()
{
  return InsertableXmlNodesProxy( std::make_pair( _childs.begin(), _childs.end() ), *this );
}

const XmlNodesProxy XmlNode::childs() const
{
  return XmlNodesProxy( std::make_pair( _childs.begin(), _childs.end() ));
}

XmlNodesProxy XmlNode::childs( const string& name )
{
  return XmlNodesProxy( _childs.equal_range( create( name ) ));
}

XmlNode* XmlNode::parent()
{
  return _parent;
}

void XmlNode::addChild( XmlNodePtr child )
{
  child->_parent = this;
  _childs.insert( child );
}
