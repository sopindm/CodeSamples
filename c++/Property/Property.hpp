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

#ifndef RTRT_PROPERTY_PROPERTY_HPP
#define RTRT_PROPERTY_PROPERTY_HPP

#include <stdint.h>
#include <string>
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <FFIObject.hpp>

namespace rtrt
{
	namespace property
	{
    
		class PropertyVisitor;
		class Property;
		typedef boost::shared_ptr< Property > PropertyPtr;
		typedef boost::weak_ptr< Property > PropertyWeakPtr;

		class UpdateHandler
		{
		public:
			virtual void update() {};

		};
		typedef boost::shared_ptr< UpdateHandler > UpdateHandlerPtr;

		class Property: public burning::FFIObject
		{
		public:
			Property( const std::string& name );
			virtual ~Property() {};

			template< class T >
			static boost::shared_ptr< T > cast( const PropertyPtr& property );

			const std::string& name() const;
			void setName( const std::string& name );

			virtual void accept( PropertyVisitor& visitor ) = 0;

			void connect( const UpdateHandlerPtr& handler );
			void disconnect( const UpdateHandlerPtr& handler );

			void signalUpdate() const;

			void setParent( const Property& parent );

			PropertyPtr link() const { return _selfLink.lock(); };

			void setSelfLink( const PropertyPtr& link ){ assert( link.get() == this ); _selfLink = link; };

		private:
			std::string _name;

			typedef std::set< UpdateHandlerPtr > HandlerSet;
			struct Handlers
			{
				HandlerSet handlers;
				boost::shared_ptr< Handlers > parentHandlers;

				void signal() const;
			};

			boost::shared_ptr< Handlers > _handlers;
			PropertyWeakPtr _selfLink;

		};

		template< class T >
		boost::shared_ptr< T > Property::cast( const PropertyPtr& property )
		{
			boost::shared_ptr< T > ret = boost::dynamic_pointer_cast< T >( property );
			assert( ret != NULL );

			return ret;
		}
	}
}
#endif
