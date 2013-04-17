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

#include "XmlSceneOStream.hpp"

using std::string;
using namespace rtrt;
using namespace rtrt::convert;

XmlSceneOStream::XmlSceneOStream( const XmlNodePtr& root ) : _root( root ), 
                                                             _current( root.get()), 
                                                             _correct( true )
{
}

const XmlNodePtr& XmlSceneOStream::root() const 
{
  return _root;
}

template< class T > 
void XmlSceneOStream::doWrite( T value, const string& name )
{
  _current->attr( name ) = value;
}

void XmlSceneOStream::write( uint32_t size, const string& name )
{
  doWrite( size, name );
}

void XmlSceneOStream::write( int value, const string& name )
{
  doWrite( value, name );
}

void XmlSceneOStream::write( float value, const string& name )
{
  doWrite( value, name );
}

void XmlSceneOStream::write( const string& value, const string& name )
{
  doWrite( value, name );
}

void XmlSceneOStream::enterSection( const string& name )
{
  XmlNodePtr child( XmlNode::create( name ));
  _current->childs() += child;
  
  _current = child.get();
}

void XmlSceneOStream::leaveSection()
{
  if( _current != NULL )
    _current = _current->parent();
  else
  {
    _correct = false;
    pushError( "Unexpected section's end." );
  }
}

bool XmlSceneOStream::correct()
{
  if( _current == NULL )
  {
    pushError( "Unexpected section's end." );
    return false;
  } 
  if( _root.get() != _current )
  {
    pushError( "Section's end missing." );
    return false;
  }
  return _correct;
}
