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

#ifndef RTRT_PROPERTY_CONSTRUCTORPROPERTY_HPP
#define RTRT_PROPERTY_CONSTRUCTORPROPERTY_HPP

#include <string>
#include <boost/weak_ptr.hpp>
#include "ListProperty.hpp"

namespace rtrt
{
	namespace property
	{
		class ConstructorProperty;
		typedef boost::shared_ptr< ConstructorProperty > ConstructorPropertyPtr;

		class ConstructorProperty: public Property
		{
		public:
			ConstructorProperty( const std::string& name );

			virtual PropertyPtr constructor() = 0;
			virtual ListPropertyPtr constructed() = 0;

			void accept( PropertyVisitor& visitor );

		protected:
			virtual void construct() = 0;

		private:
			class ConstructorUpdateHandler;

			const unsigned char* type_uuid() const;

		};

		template< class ValueType, class Constructor >
		class SimpleConstructorProperty: public ConstructorProperty
		{
		public:
			static ConstructorPropertyPtr create( const std::string& name, 
												  ValueType& value, 
												  Constructor constructor = Constructor() );

			PropertyPtr constructor();
			ListPropertyPtr constructed();

		private:
			void construct();

			SimpleConstructorProperty( const std::string& name, 
									   ValueType& value,
									   Constructor constructor = Constructor() );

			SimpleConstructorProperty( const SimpleConstructorProperty& );
			void operator=( const SimpleConstructorProperty& );

			ValueType& _value;
			Constructor _constructor;

		};
	}
}

#include "ConstructorProperty_impl.hpp"
#endif
