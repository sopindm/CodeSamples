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

#ifndef RTRT_PROPERTY_PROPERTIZED_HPP
#define RTRT_PROPERTY_PROPERTIZED_HPP

#include "AtomProperty.hpp"
#include "ListProperty.hpp"

namespace rtrt
{
	namespace property
	{
		class Propertized
		{
		public:
			virtual ~Propertized() {};

			virtual ListPropertyPtr properties() = 0;

			template< class T >
			T property( const std::string& name ) const;

			template< class T >
			void setProperty( const std::string& name, const T& value );

		protected:
			virtual void onUpdate( const Property& property ) {};

		};

		template< class T >
		T Propertized::property( const std::string& name ) const
		{
			ListPropertyPtr props( const_cast< Propertized* >( this )->properties() );

			return Property::cast< TypedProperty< T > >( props->at( name ) )->get();
		}

		template< class T >
		void Propertized::setProperty( const std::string& name, const T& value )
		{
			Property::cast< TypedProperty< T > >( properties()->at( name ) )->set( value );
		}
	}
}
#endif
