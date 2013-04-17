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

#ifndef RTRT_PROPERTY_LISTPROPERTY_HPP
#define RTRT_PROPERTY_LISTPROPERTY_HPP

#include <vector>
#include "Property.hpp"

namespace rtrt
{
  namespace property
  {
    class ListProperty;
    typedef boost::shared_ptr< ListProperty > ListPropertyPtr;

    class ListProperty: public Property
    {
    public:
      static ListPropertyPtr create( const std::string& name, const std::vector< PropertyPtr >& properties );

      ListPropertyPtr append( const PropertyPtr& property ) const;

      ListPropertyPtr append( const PropertyPtr& property1,
			      const PropertyPtr& property2 ) const;

      uint32_t size() const;
      const PropertyPtr& at( uint32_t position );
      const PropertyPtr& at( const std::string& name );

      uint32_t position( const std::string& name );

      void accept( PropertyVisitor& visitor );

      const unsigned char* type_uuid() const;

    protected:
      static ListPropertyPtr doCreate( ListProperty* property );

      ListProperty( const std::string& name, const std::vector< PropertyPtr >& properties );
      void push( const PropertyPtr& property ) { _properties.push_back( property ); };

    private:
      ListProperty( const ListProperty& );
      void operator=( const ListProperty& );

      std::vector< PropertyPtr > _properties;

    };

    class ColorProperty: public ListProperty
    {
    public:
      static ListPropertyPtr create( const std::string& name, const std::vector< PropertyPtr >& properties );

    private:
      ColorProperty( const std::string& name, const std::vector< PropertyPtr >& properties ):
	ListProperty( name, properties ) {}
    };
  }
}
#endif
