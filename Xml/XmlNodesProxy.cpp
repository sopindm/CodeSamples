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

#include "XmlNode.hpp"
#include "XmlNodesProxy.hpp"

using std::pair;
using std::string;
using namespace rtrt;

XmlNodesProxy::XmlNodesProxy( const NodeRange& range ) : _range( range )
{
}

bool XmlNodesProxy::exist() const
{
  return _range.first != _range.second;
}

size_t XmlNodesProxy::count() const
{
  return std::distance( _range.first, _range.second );
}

InsertableXmlNodesProxy::InsertableXmlNodesProxy( const NodeRange& range, XmlNode& node ): 
                                                       XmlNodesProxy( range ), _node( node )
{
}

void InsertableXmlNodesProxy::operator+=( const XmlNodePtr& ptr )
{
  _node.addChild( ptr );
}

void InsertableXmlNodesProxy::operator-=( const XmlNodePtr& ptr )
{
  for( NodeSet::iterator iter = begin(); iter != end(); iter++ )
  {
    if( *iter == ptr )
      _node._childs.erase( iter );
  }
}

NodeSet::iterator XmlNodesProxy::begin()
{
  return _range.first;
}

NodeSet::const_iterator XmlNodesProxy::begin() const
{
  return _range.first;
}

NodeSet::iterator XmlNodesProxy::end()
{
  return _range.second;
}

NodeSet::const_iterator XmlNodesProxy::end() const
{
  return _range.second;
}
