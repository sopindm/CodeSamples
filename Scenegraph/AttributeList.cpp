/*
   -----------------------------------------------------------------------------
   |                 R T R T    S C E N E G R A P H   L I B R A R Y                 |
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
#include <algorithm>
#include "AttributeList.hpp"

using std::string;
using namespace rtrt::scenegraph;

AttributeList::AttributeList(): _attrs()
{
}

AttributeList::AttributeList( const AttributeList& list )
{
  copyList( list );
}

void AttributeList::operator=( const AttributeList& list )
{
  _attrs.clear();
  copyList( list );
}

void AttributeList::copyList( const AttributeList& list )
{
  std::pair< std::string, AttributePtr > attr;
  BOOST_FOREACH( attr, list._attrs )
  {
    _attrs[ attr.first ] = attr.second->clone();
  }
}

void AttributeList::setAttribute( const string& name, const Attribute& value )
{
  _attrs[ name ] = value.clone();
}

AttributeList AttributeList::merge( const AttributeList& list ) const
{
  AttributeList ret( *this );

  std::pair< std::string, AttributePtr > attr;
  BOOST_FOREACH( attr, list._attrs )
  {
    ret.mergeAttribute( attr.first, *attr.second );
  }

  return ret;
}

AttributeList AttributeList::delta( const AttributeList& list ) const
{
  AttributeList ret( *this );

  std::pair< std::string, AttributePtr > attr;
  BOOST_FOREACH( attr, list._attrs )
  {
    AttributeMap::iterator it( ret._attrs.find( attr.first ));

    if( it != ret._attrs.end() )
      ret._attrs.erase( it );
  }

  return ret;
}

void AttributeList::touch( const AttributeList& list )
{
  std::pair< std::string, AttributePtr > attr;
  BOOST_FOREACH( attr, list._attrs )
  {
    touchAttribute( attr.first, *attr.second );
  }
}

void AttributeList::touchAttribute( const string& name, const Attribute& oldValue )
{
  AttributeMap::iterator iter( _attrs.find( name ));

  if( iter == _attrs.end() )
  {
    AttributePtr newAttr( oldValue.defaultValue() );
    newAttr->setNew();

    _attrs[ name ] = newAttr;
  }
  else
    _attrs[ name ]->setNew();
}

void AttributeList::mergeAttribute( const string& name, const Attribute& value )
{
  AttributeMap::iterator iter( _attrs.find( name ));

  if( iter == _attrs.end() )
    _attrs[ name ] = value.clone();
  else
    _attrs[ name ]->merge( value );
}

AttributePtr AttributeList::attribute( const string& name ) const
{
  AttributeMap::const_iterator iter( _attrs.find( name ));
  if( iter == _attrs.end() )
    return AttributePtr();
    
  return iter->second;
}
