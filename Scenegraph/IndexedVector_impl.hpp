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
    template< class V >
    IndexedVector< V >::IndexedVector(): _vector(),
					 _offsets( 0 )
    {
    }

    template< class V >
    void IndexedVector< V >::insertIndex( size_t index )
    {
      _offsets.insert( index );
    }

    template< class V >
    void IndexedVector< V >::eraseIndex( size_t index )
    {
      _offsets.erase( index );
    }

    template< class V >
    void IndexedVector< V >::allocate( size_t index, size_t size )
    {
      _offsets.allocate( index, size );
      _vector.insert( _vector.begin() + _offsets.offset( index ), size, typename V::value_type() );
    }

    template< class V >
    void IndexedVector< V >::free( size_t index )
    {
      size_t offset = _offsets.offset( index );
      size_t size = _offsets.size( index );
      _offsets.free( index );

      _vector.erase( _vector.begin()+offset, _vector.begin()+offset+size );
    }

    template< class V >
    V& IndexedVector< V >::value() { return _vector; };

    template< class V >
    const V& IndexedVector< V >::value() const { return _vector; };

    template< class V >
    std::vector< size_t >& IndexedVector< V >::offsets() { return _offsets.value(); };

    template< class V >
    const std::vector< size_t >& IndexedVector< V >::offsets() const { return _offsets.value(); };

    template< class V >
    typename V::value_type& IndexedVector< V >::at( size_t index )
    {
      return _vector[ _offsets.offset( index ) ];
    }

    template< class V >
    const typename V::value_type& IndexedVector< V >::at( size_t index ) const
    {
      return _vector[ _offsets.offset( index ) ];
    }

    template< class V >
    size_t IndexedVector< V >::offset( size_t id ) const
    {
      return _offsets.offset( id );
    }

    template< class V >
    size_t IndexedVector< V >::size( size_t id ) const
    {
      return _offsets.size( id );
    }
  }
}
