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

namespace rtrt
{
  namespace property
  {
    template< class T, class Constructor, bool b >
    VectorPropertyPtr SimpleVectorProperty< T, Constructor, b >::create( const std::string& name, 
									      const std::string& elementName,
									      typename Type< T >::VectorType& value,
									      const Constructor& constructor )
    {
      VectorPropertyPtr ret( new SimpleVectorProperty( name, elementName, value, constructor ));
      ret->setSelfLink( ret );

      return ret;
    }

    template< class T, class Constructor, bool b >
    SimpleVectorProperty< T, Constructor, b >::SimpleVectorProperty( const std::string& name, 
								     const std::string& elementName,
								     typename Type< T >::VectorType& value,
								     const Constructor& constructor ):
      VectorProperty( name ),
      _elementName( elementName ),
      _data( value ),
      _constructor( constructor )
    {
    }

    template< class T, class C, bool b >
    uint32_t SimpleVectorProperty< T, C, b >::size() const
    {
      return _data.size();
    }

    template< class T, class C, bool b >
    PropertyPtr SimpleVectorProperty< T, C, b >::at( uint32_t index )
    {
      PropertyPtr ret( _constructor.propertize( _data.at( index ), _elementName ) );
      ret->setParent( *this );
      
      return ret;
    }

    template< class T, class C, bool b >
    void SimpleVectorProperty< T, C, b >::remove( uint32_t index )
    {
      assert( index >= 0 && index < _data.size() );

      _data.erase( _data.begin() + index );
      signalUpdate();
    }

    template< class T, class C, bool b >
    void SimpleVectorProperty< T, C, b >::insert( uint32_t index )
    {
      assert( index >= 0 && index <= _data.size() );

      _data.insert( _data.begin() + index, _constructor.create() );
      signalUpdate();
    }

    template< class T, class C, bool b >
    void SimpleVectorProperty< T, C, b >::pushBack()
    {
      _data.push_back( _constructor.create() );
      signalUpdate();
    }

    namespace impl
    {
      template< class T, bool haveRawAccess = Type< T >::haveRawAccess >
      struct RawAccessor;

      template< class T >
      struct RawAccessor< T, true >
      {
	template< class C >
	static void resize( typename Type< T >::VectorType& vector, C& constructor, size_t newSize )
	{
	  vector.resize( newSize, constructor.create() );
	}

	static void* access( typename Type< T >::VectorType& vector ) {  return vector.data(); };
	static uint32_t elementSize() { return sizeof( T ); };
      };

      template< class T >
      struct RawAccessor< T, false >
      {
	template< class C >
	static void resize( typename Type< T >::VectorType& vector, C& constructor, size_t newSize )
	{
	  vector.resize( newSize );

	  for( size_t i=0; i<vector.size(); i++ )
	    vector[ i ] = constructor.create();
	}

	static void* access( typename Type< T >::VectorType& ) {  return NULL; };
	static uint32_t elementSize() { return 0; };
      };
    }

    template< class T, class C, bool haveRawAccess >
    void SimpleVectorProperty< T, C, haveRawAccess >::resize( uint32_t newSize )
    {
      impl::RawAccessor< T, haveRawAccess >::resize( _data, _constructor, newSize );
      signalUpdate();
    }


    template< class T, class C, bool haveRawAccess >
    void* SimpleVectorProperty< T, C, haveRawAccess >::rawData()
    {
      return impl::RawAccessor< T, haveRawAccess >::access( _data );
    }

    template< class T, class C, bool b >
    uint32_t SimpleVectorProperty< T, C, b >::elementSize() const
    {
      return impl::RawAccessor< T >::elementSize();
    }
  }
}
