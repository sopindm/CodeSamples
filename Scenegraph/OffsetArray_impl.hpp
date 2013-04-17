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

namespace rtrt
{
  namespace scenegraph
  {
    template< class T >
    void OffsetArray< T >::insert( size_t index )
    {
      assert( index < _offsets.size() );

      if( index < _offsets.size() - 1 )
	return;

      _offsets.push_back( _offsets[ _offsets.size() - 1 ] );
    }

    template< class T >
    void OffsetArray< T >::erase( size_t index )
    {
      assert( index < _offsets.size() - 1 );
      free( index );

      _offsets.erase( _offsets.begin() + index );
    }

    template< class T >
    void OffsetArray< T >::allocate( size_t index, T size )
    {
      assert( index < _offsets.size() - 1 );
      assert( _offsets[ index ] == _offsets[ index + 1 ] );
      
      for( size_t i = index + 1; i<_offsets.size(); i++ )
	_offsets[ i ] += size;
    }

    template< class T>
    void OffsetArray< T >::free( size_t index )
    {
      assert( index < _offsets.size() - 1 );

      T deleteSize( size( index ));

      for( size_t i=index+1; i<_offsets.size(); i++ )
	_offsets[ i ] -= deleteSize;
    }

    template< class T>
    T OffsetArray< T >::offset( size_t index ) const
    {
      assert( index < _offsets.size() );

      return _offsets[ index ];
    }

    template< class T >
    T OffsetArray< T >::size( size_t index ) const
    {
      assert( index < _offsets.size() - 1 );

      return _offsets[ index + 1 ] - _offsets[ index ];
    }

    template< class T >
    std::vector< T >& OffsetArray< T >::value()
    {
      return _offsets;
    }

    template< class T >
    const std::vector< T >& OffsetArray< T >::value() const
    {
      return _offsets;
    }
  }
}
