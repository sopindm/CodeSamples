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

#include "Attribute.hpp"

using namespace Eigen;
using namespace rtrt::scenegraph;

Attribute::Attribute( const Time& time ): Timed( time )
{
}

Attribute::Attribute( const Attribute& attribute, const Time& time ): 
  Timed( attribute )
{
  if( isOld( time ) )
  {
    setNew();
    resolveNewest( time );
  }
}

Attribute::~Attribute()
{
}

void Attribute::merge( const Attribute& newAttribute )
{
  if( newAttribute.isNew( time() ))
  {
    setNew();
    resolveNewest( newAttribute.time() );
  }
}

TransformAttribute::TransformAttribute( const Time& time ): 
  Attribute( time ),
  _transform( static_cast< Transform3f >( Transform3f::Identity() ))
{
}

TransformAttribute::TransformAttribute( const Transform3f& transform, const Time& time ): 
  Attribute( time ),
  _transform( transform )
{
}

TransformAttribute::TransformAttribute( const TransformAttribute& attr ):
  Attribute( attr, attr.time() ),
  _transform( attr._transform )
{
}

const Transform3f& TransformAttribute::value() const
{
  return _transform;
}

void TransformAttribute::setValue( const Transform3f& value )
{
  _transform = value;
  setNew();
}

AttributePtr TransformAttribute::clone() const
{
  return AttributePtr( new TransformAttribute( *this ) );
}

void TransformAttribute::merge( const Attribute& attribute )
{
  const TransformAttribute* attr( dynamic_cast< const TransformAttribute* >( &attribute) );
  assert( attr != NULL );

  _transform = _transform * attr->_transform;
  Attribute::merge( attribute );
}

VisibilityAttribute::VisibilityAttribute( const Time& time ): 
  Attribute( time ),
  _isVisible( true )
{
}

VisibilityAttribute::VisibilityAttribute( bool value, const Time& time ): 
  Attribute( time ),
  _isVisible( value )
{
}

VisibilityAttribute::VisibilityAttribute( const VisibilityAttribute& attr ):
  Attribute( attr, attr.time() ),
  _isVisible( attr._isVisible )
{
}

bool VisibilityAttribute::value() const
{
  return _isVisible;
}

void VisibilityAttribute::setValue( bool value )
{
  if( _isVisible != value )
  {
    _isVisible = value;
    setNew();
  }
}

void VisibilityAttribute::merge( const Attribute& newAttribute )
{
  const VisibilityAttribute* attr( dynamic_cast< const VisibilityAttribute* >( &newAttribute ));
  assert( attr );

  _isVisible = _isVisible && attr->value();
  Attribute::merge( newAttribute );
}

AttributePtr VisibilityAttribute::clone() const
{
  return AttributePtr( new VisibilityAttribute( *this ));
}
