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

#ifndef RTRT_PROPERTY_VECTORPROPERTY_HPP
#define RTRT_PROPERTY_VECTORPROPERTY_HPP

#include "Property.hpp"
#include "TypeTraits.hpp"
#include "PropertyBuilder.hpp"

namespace rtrt
{
  namespace property
  {
    class VectorProperty: public Property
    {
    public:
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW

      VectorProperty( const std::string& name );

      virtual uint32_t size() const = 0;
      virtual PropertyPtr at( uint32_t position ) = 0;

      virtual void remove( uint32_t position ) = 0;
      virtual void insert( uint32_t position ) = 0;
      virtual void pushBack() = 0;

      virtual void resize( uint32_t newSize ) = 0;

      virtual void* rawData();
      virtual uint32_t elementSize() const;

      void accept( PropertyVisitor& visitor );

      const unsigned char* type_uuid() const;

    };

    typedef boost::shared_ptr< VectorProperty > VectorPropertyPtr;

    template< class T >
    class VectorConstructor
    {
    public:
      VectorConstructor( T defaultValue = T() ):
	_value( defaultValue )
      {
      };

      T create() const { return _value; };
      
      static PropertyPtr propertize( T& value, const std::string& name )
      {
	return buildProperty( name, value );
      };

    private:
      T _value;

    };

    template<>
    class VectorConstructor< bool >
    {
    public:
      VectorConstructor( bool value = false ): _value( value ) {};

      bool create() const { return _value; };

      static PropertyPtr propertize( std::vector< bool >::reference value, const std::string& name )
      {
	return buildProperty( name,  value );
      }

    private:
      bool _value;

    };

    template< class T, class Constructor = VectorConstructor< T >, bool withRawAccess = Type< T >::haveRawAccess >
    class SimpleVectorProperty: public VectorProperty
    {
    public:
      static VectorPropertyPtr create( const std::string& name, 
				       const std::string& elementName,
				       typename Type< T >::VectorType& _data,
				       const Constructor& constructor = Constructor() );

      uint32_t size() const;
      PropertyPtr at( uint32_t position );

      void remove( uint32_t position );
      void insert( uint32_t position );
      void pushBack();

      void resize( uint32_t newSize );

      void* rawData();
      uint32_t elementSize() const;

      const std::string& elementName() const;
      void setElementName( const std::string& name );

    private:
      SimpleVectorProperty( const std::string& name, 
			    const std::string& elementName,
			    typename Type< T >::VectorType& _data,
			    const Constructor& constructor = Constructor() );

      std::string _elementName;

      typename Type< T >::VectorType& _data;
      Constructor _constructor;

    };

    template< class T >
    PropertyPtr buildProperty( const std::string& name, 
			       const std::string& elementName, 
			       typename Type< T >::VectorType& vector )
    {
      return PropertyPtr( SimpleVectorProperty< T >::create( name, elementName, vector ));
    }
  }
}

#include "VectorProperty_impl.hpp"
#endif
