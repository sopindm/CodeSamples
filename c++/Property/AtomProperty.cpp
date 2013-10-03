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

#include "AtomProperty.hpp"
#include "PropertyVisitor.hpp"
#include "property_uuid.uuid.h"

using std::string;
using std::vector;
using namespace rtrt::property;

namespace rtrt
{
  namespace property
  {
    template<>
    void rtrt::property::BoolProperty::accept( PropertyVisitor& visitor )
    {
      visitor.visit( *this );
    }

    template<>
    void rtrt::property::IntProperty::accept( PropertyVisitor& visitor )
    {
      visitor.visit( *this );
    }

    template<>
    void rtrt::property::UIntProperty::accept( PropertyVisitor& visitor )
    {
      visitor.visit( *this );
    }

    template<>
    void rtrt::property::FloatProperty::accept( PropertyVisitor& visitor )
    {
      visitor.visit( *this );
    }

    template<>
    void rtrt::property::StringProperty::accept( PropertyVisitor& visitor )
    {
      visitor.visit( *this );
    }

    BoolPropertyPtr BoolReferenceProperty::create( const string& name, vector< bool >::reference value )
    {
      BoolPropertyPtr ret( new BoolReferenceProperty( name, value ));
      ret->setSelfLink( ret );

      return ret;
    }

    BoolReferenceProperty::BoolReferenceProperty( const string& name, vector< bool >::reference value ):
      BoolProperty( name ),
      _value( value )
    {
    }

    bool BoolReferenceProperty::get() const
    {
      return _value;
    }

    void BoolReferenceProperty::set( const bool& value ) 
    {
      signalUpdate();
      _value = value;
    }

    bool BoolReferenceProperty::isReadOnly() const
    {
      return false;
    }

    template<>
    const unsigned char* rtrt::property::BoolProperty::type_uuid() const
    {
      return bool_property_uuid;
    }

    template<>
    const unsigned char* rtrt::property::IntProperty::type_uuid() const
    {
      return int_property_uuid;
    }

    template<>
    const unsigned char* rtrt::property::UIntProperty::type_uuid() const
    {
      return uint_property_uuid;
    }

    template<>
    const unsigned char* rtrt::property::FloatProperty::type_uuid() const
    {
      return float_property_uuid;
    }

    template<>
    const unsigned char* rtrt::property::StringProperty::type_uuid() const
    {
      return string_property_uuid;
    }
  }
}

