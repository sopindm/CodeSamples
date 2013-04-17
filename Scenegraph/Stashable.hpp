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

#ifndef RTRT_SCENEGRAPH_STASHABLE_HPP
#define RTRT_SCENEGRAPH_STASHABLE_HPP

#include <string>
#include <vector>
#include "Kernel.hpp"

namespace rtrt
{
  namespace scenegraph
  {

    /*! Base interface for stashable classes. */
    class BaseStashable: virtual public Kernel
    {
    public:
      /*! Names of used stashes */
      const std::vector< std::string > stashes() { return _stashes; };
      virtual ~BaseStashable() {};

    protected:
      /*! Names of used stashes */
      std::vector< std::string > _stashes; 
      
    };

    template< class T >
    /*! An implementation part */
    class OnlyStashable : public T::StashInterface, public BaseStashable
    {
    public:
      OnlyStashable()
      {
	_stashes.push_back( T::stashName() );
      }
    };

    namespace impl
    {
      /*! An implementation part */
      struct Empty{};
    }

    /*! An implementation part */
    template< class T, class Base >
    class AdditionalStashable : public Base, public T::StashInterface
    {
    public:
      AdditionalStashable(): Base()
      {
	Base::_stashes.push_back( T::stashName() );
      }
    };

    /*! An implementation part */
    template< class T >
    class AdditionalStashable< impl::Empty, T > : public T
    {
    };

    /*! An implementation part */
    template< class T >
    class Stashable1 : public OnlyStashable< T > {};

    /*! An implementation part */
    template< class T1, class T2 >
    class Stashable2 : public AdditionalStashable< T2, Stashable1< T1 > > {};

    /*! An implementation part */
    template< class T1, class T2, class T3 >
    class Stashable3 : public AdditionalStashable< T3, Stashable2< T1, T2 > > {};

    /*! An implementation part */
    template< class T1, class T2, class T3, class T4 >
    class Stashable4 : public AdditionalStashable< T4, Stashable3< T1, T2, T3 > > {};

    /*! Class for stashable objects. Templatized with stash classes. */
    template< class T1, 
	      class T2 = impl::Empty, 
	      class T3 = impl::Empty, 
	      class T4 = impl::Empty, 
	      class T5 = impl::Empty >
    class Stashable : public AdditionalStashable< T5, Stashable4< T1, T2, T3, T4 > > {};

  }
}
#endif
