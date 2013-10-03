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

#ifndef RTRT_PROPERTY_ATOMPROPERTY_HPP
#define RTRT_PROPERTY_ATOMPROPERTY_HPP

#include <vector>
#include "Property.hpp"

namespace rtrt
{
  namespace property
  {
    class AtomProperty: public Property
    {
    public:
      AtomProperty( const std::string& name ): Property( name ) {};

      template< class T >
      static void set( const PropertyPtr& property, const T& value );

      template< class T >
      static T get( const PropertyPtr& property );

      virtual bool isReadOnly() const = 0;
    };

    template< class T >
    class TypedProperty: public AtomProperty
    {
    public:
      TypedProperty( const std::string& name ): AtomProperty( name ) {};

      virtual T get() const = 0;
      virtual void set( const T& value ) = 0;
      virtual T* access() { return NULL; };

      virtual const T* lowerBound() const { return NULL; };
      virtual const T* upperBound() const { return NULL; };

      virtual std::vector< T > possibleValues() { return std::vector< T >(); };

      void accept( PropertyVisitor& visitor );

      const unsigned char* type_uuid() const;
    };

    typedef TypedProperty< bool > BoolProperty;
    typedef TypedProperty< int32_t > IntProperty;
    typedef TypedProperty< uint32_t > UIntProperty;
    typedef TypedProperty< float > FloatProperty;
    typedef TypedProperty< std::string > StringProperty;

    typedef boost::shared_ptr< BoolProperty > BoolPropertyPtr;
    typedef boost::shared_ptr< IntProperty > IntPropertyPtr;
    typedef boost::shared_ptr< UIntProperty > UIntPropertyPtr;
    typedef boost::shared_ptr< FloatProperty > FloatPropertyPtr;
    typedef boost::shared_ptr< StringProperty > StringPropertyPtr;

    namespace impl
    {
      template< class T, bool isReadonly >
      struct Accessor;

      template< class T >
      struct Accessor< T, false >
      {
	typedef T& ValueType;

	static void set( T& place, const T& value ) { place = value; };
	static T* access( T& value ) { return &value; };
      };

      template< class T >
      struct Accessor< T, true >
      {
	typedef T ValueType;

	static void set( T& place, const T& value ) {};
	static T* access( T& value ) { return NULL; };
      };
    }

    template< class T, bool isReadonly = false >
    class SimpleProperty: public TypedProperty< T >
    {
    public:
      static boost::shared_ptr< TypedProperty< T > > create( 
	const std::string& name, 
	typename impl::Accessor< T, isReadonly >::ValueType value );

      T get() const;
      void set( const T& value );
      T* access() { return impl::Accessor< T, isReadonly >::access( _value ); };

      bool isReadOnly() const { return isReadonly; };

    protected:
      SimpleProperty( const std::string& name, typename impl::Accessor< T, isReadonly >::ValueType value );

    private:
      SimpleProperty( const SimpleProperty< T >& property );
      void operator=( const SimpleProperty< T >& );

      typename impl::Accessor< T, isReadonly >::ValueType _value;

    };

    class BoolReferenceProperty: public BoolProperty
    {
    public:
      static BoolPropertyPtr create( const std::string& name, std::vector< bool >::reference value );

      bool get() const;
      void set( const bool& value );

      bool isReadOnly() const;

    private:
      BoolReferenceProperty( const std::string& name, std::vector< bool >::reference value );

      std::vector< bool >::reference _value;

    };

    template< class T >
    class RangedProperty: public SimpleProperty< T >
    {
    public:
      static boost::shared_ptr< TypedProperty< T > > create( const std::string& name,
							     T& value,
							     const T& minValue,
							     const T& maxValue );

      void set( const T& value ) { if( value >= _minValue && value <= _maxValue ) SimpleProperty< T >::set( value ); };

      const T* lowerBound() const { return &_minValue; };
      const T* upperBound() const { return &_maxValue; };

    private:
      RangedProperty( const std::string& name, T& value, const T& minValue, const T& maxValue );

      T _minValue;
      T _maxValue;

    };

    template< class T >
    class ListedValueProperty: public SimpleProperty< T >
    {
    public:
      static boost::shared_ptr< TypedProperty< T > > create( const std::string& name,
							     T& value,
							     const std::vector< T >& values );
      
      void set( const T& value );

      std::vector< T > possibleValues() { return _values; };

    private:
      ListedValueProperty( const std::string& name, T& value, const std::vector< T >& values );

      std::vector< T > _values;

    };
  }
}

#include "AtomProperty_impl.hpp"
#endif
