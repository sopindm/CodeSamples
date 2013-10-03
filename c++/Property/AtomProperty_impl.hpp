/*
   -----------------------------------------------------------------------------
   |                 R T R T    P R O P E R T Y  L I B R A R Y                 |
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

#include <algorithm>

namespace rtrt
{
  namespace property
  {
    template< class T >
    void AtomProperty::set( const PropertyPtr& property, const T& value )
    {
      Property::cast< TypedProperty< T > >( property )->set( value );
    }

    template< class T >
    T AtomProperty::get( const PropertyPtr& property )
    {
      return Property::cast< TypedProperty< T > >( property )->get();
    }

    template< class T, bool isReadonly >
    boost::shared_ptr< TypedProperty< T > > SimpleProperty< T, isReadonly >::create(
      const std::string& name,
      typename impl::Accessor< T, isReadonly >::ValueType value )
    {
      boost::shared_ptr< TypedProperty< T > > ret( 
	new SimpleProperty< T, isReadonly >( name, value ) );

      ret->setSelfLink( ret );

      return ret;
    }

    template< class T, bool isReadonly >
    SimpleProperty< T, isReadonly >::SimpleProperty( const std::string& name, 
						     typename impl::Accessor< T, isReadonly >::ValueType value ):
      TypedProperty< T >( name ),
      _value( value )
    {
    }

    template< class T, bool isReadonly >
    T SimpleProperty< T, isReadonly >::get() const
    {
      return _value;
    }

    template< class T, bool isReadonly >
    void SimpleProperty< T, isReadonly >::set( const T& value )
    {
      impl::Accessor< T, isReadonly >::set( _value, value ); 
      Property::signalUpdate();
    }

    template< class T >
    boost::shared_ptr< TypedProperty< T > > RangedProperty< T >::create( const std::string& name,
									 T& value, 
									 const T& minValue,
									 const T& maxValue )
    {
      boost::shared_ptr< TypedProperty< T > > ret( new RangedProperty< T >( name, value, minValue, maxValue ));
      ret->setSelfLink( ret );

      return ret;
    }

    template< class T >
    RangedProperty< T >::RangedProperty( const std::string& name, 
					 T& value, 
					 const T& lowerBound, const T& upperBound ):
      SimpleProperty< T >( name, value ),
      _minValue( lowerBound ),
      _maxValue( upperBound )
    {
    }

    template< class T >
    boost::shared_ptr< TypedProperty< T > > ListedValueProperty< T >::create( const std::string& name,
									      T& value, 
									      const std::vector< T >& possible )
    {
      boost::shared_ptr< TypedProperty< T > > ret( new ListedValueProperty< T >( name, value, possible ));
      ret->setSelfLink( ret );

      return ret;
    }

    template< class T >
    ListedValueProperty< T >::ListedValueProperty( const std::string& name, 
						   T& value, 
						   const std::vector< T >& possibleValues ):
      SimpleProperty< T >( name, value ),
      _values( possibleValues )
    {
    }

    template< class T >
    void ListedValueProperty< T >::set( const T& value )
    {
      typename std::vector< T >::iterator place( std::find( _values.begin(), _values.end(), value ));
      
      if( place != _values.end() )
	SimpleProperty< T >::set( value );
    }
  }
}
