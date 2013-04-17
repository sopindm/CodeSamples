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

#include "XmlSceneIStream.hpp"

using std::string;
using namespace rtrt;
using namespace rtrt::convert;

XmlSceneIStream::XmlSceneIStream( const XmlNodePtr& root ) : _root( root ),
                                                             _current( _root.get() ), 
                                                             _childs(),
                                                             _correct( true )
{
  _childs.push( ChildNodes() );
}

void XmlSceneIStream::setRoot( XmlNodePtr root )
{
  _root = root;
  _current = root.get();
}

XmlNode* XmlSceneIStream::current()
{
  return _current;
}

template< class T >
void XmlSceneIStream::doRead( T* value, const string& name )
{
  if( !_current->attr( name ).isSet() )
  {
    _correct = false;
    pushError( "Attribute " + name + " not set." );
  }
  
  *value = _current->attr( name ).as< T >();
}

void XmlSceneIStream::read( int* value, const string& name )
{
  doRead( value, name );
}

void XmlSceneIStream::read( float* value, const string& name )
{
  doRead( value, name );
}

void XmlSceneIStream::read( uint32_t* size, const string& name )
{
  doRead( size, name );
}

void XmlSceneIStream::read( string* value, const string& name )
{
  doRead( value, name );
}

void XmlSceneIStream::enterSection( const string& name )
{
  ChildNodes::const_iterator child = _childs.top().find( name );
  if( child == _childs.top().end() )
  {
    _childs.top()[ name ] = std::make_pair( _current->childs( name ).begin(),
	        			    _current->childs( name ).end() );
  }
  
  NodeSet::const_iterator curr = _childs.top()[ name ].first;
  if( curr == _childs.top()[ name ].second )
  {
    _correct = false;
    pushError( "Current node has not enought childs with name " + name + "." );
  }
  else
  {
    _current = curr->get();
    _childs.top()[ name ].first++;
    _childs.push( ChildNodes() );
  }
}

void XmlSceneIStream::leaveSection() 
{
  if( !_childs.empty() )
    _childs.pop();
  if( _current == NULL )
  {
    _correct = false;
    pushError( "Unexpected section's end." );
  }
  else 
    _current = _current->parent();
}

bool XmlSceneIStream::correct()
{
  if( _current == NULL )
  {
    pushError( "Unexpected section's end." );
    _correct = false;
  }
  else if( _current != _root.get() )
  {
    pushError( "Section's end missing." );
    _correct = false;
  }
  return _correct;
}
