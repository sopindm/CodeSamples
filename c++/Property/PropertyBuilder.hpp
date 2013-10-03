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

#ifndef RTRT_PROPERTY_PROPERTYBUILDER_HPP
#define RTRT_PROPERTY_PROPERTYBUILDER_HPP

#include <string>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "AtomProperty.hpp"
#include "ListProperty.hpp"
#include "ConstructorProperty.hpp"

namespace rtrt
{
	namespace property
	{
		template< class T >
		PropertyPtr buildProperty( const std::string& name, T& value );

		template< class T >
		PropertyPtr buildProperty( const std::string& name, const T& value );

		ListPropertyPtr buildListProperty( const std::string& name );

		ListPropertyPtr buildListProperty( const std::string& name, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&,
										   const PropertyPtr& );

		ListPropertyPtr buildListProperty( const std::string& name,
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&, 
										   const PropertyPtr&,
										   const PropertyPtr&,
										   const PropertyPtr& );

		namespace impl
		{
			template< class T, bool isReadonly >
			struct LinkType;

			template< class T >
			struct LinkType< T, true >
			{
				typedef const T& Value;
			};

			template< class T >
			struct LinkType< T, false >
			{
				typedef T& Value;
			};

			template< class T, bool isReadonly >
			struct PropertyBuilder
			{
				static PropertyPtr build( const std::string& name, typename LinkType< T, isReadonly >::Value value )
					{
						return SimpleProperty< T, isReadonly >::create( name, value );
					}
			};

			template< bool isReadonly >
			struct PropertyBuilder< Eigen::Vector2f, isReadonly >
			{
				static PropertyPtr build( const std::string& name, 
										  typename LinkType< Eigen::Vector2f, isReadonly >::Value value )
					{
						return buildListProperty( name,
												  buildProperty( "x", value.x() ),
												  buildProperty( "y", value.y() ));
					}
			};

			template< bool isReadonly >
			struct PropertyBuilder< Eigen::Vector3f, isReadonly >
			{
				static PropertyPtr build( const std::string& name, 
										  typename LinkType< Eigen::Vector3f, isReadonly >::Value value )
					{
						return buildListProperty( name,
												  buildProperty( "x", value.x() ),
												  buildProperty( "y", value.y() ),
												  buildProperty( "z", value.z() ) );
					}
			};

			template< bool isReadonly >
			struct PropertyBuilder< Eigen::Vector4f, isReadonly >
			{
				static PropertyPtr build( const std::string& name, 
										  typename LinkType< Eigen::Vector4f, isReadonly >::Value value )
					{
						return buildListProperty( name,
												  buildProperty( "x", value.x() ),
												  buildProperty( "y", value.y() ),
												  buildProperty( "z", value.z() ),
												  buildProperty( "w", value.w() ) );
					}
			};

			template< bool isReadonly >
			struct PropertyBuilder< Eigen::Vector3i, isReadonly >
			{
				static PropertyPtr build( const std::string& name, 
										  typename LinkType< Eigen::Vector3i, isReadonly >::Value value )
					{
						return buildListProperty( name,
												  buildProperty( "v1", value.x() ),
												  buildProperty( "v2", value.y() ),
												  buildProperty( "v3", value.z() ) );
					}
			};

			template< bool isReadonly >
			struct PropertyBuilder< Eigen::Vector4i, isReadonly >
			{
				static PropertyPtr build( const std::string& name, 
										  typename LinkType< Eigen::Vector4i, isReadonly >::Value value )
					{
						return buildListProperty( name,
												  buildProperty( "v1", value.x() ),
												  buildProperty( "v2", value.y() ),
												  buildProperty( "v3", value.z() ),
												  buildProperty( "v4", value.w() ) );
					}
			};

			template< bool isReadonly >
			struct PropertyBuilder< Eigen::Transform3f, isReadonly >
			{
				static PropertyPtr build( const std::string& name, 
										  typename LinkType< Eigen::Transform3f, isReadonly >::Value value )
					{
						return buildListProperty( name,
												  buildListProperty( "row1", 
																	 buildProperty( "x", value( 0, 0 )),
																	 buildProperty( "y", value( 0, 1 )),
																	 buildProperty( "z", value( 0, 2 )),
																	 buildProperty( "w", value( 0, 3 )) ),
												  buildListProperty( "row2", 
																	 buildProperty( "x", value( 1, 0 )),
																	 buildProperty( "y", value( 1, 1 )),
																	 buildProperty( "z", value( 1, 2 )),
																	 buildProperty( "w", value( 1, 3 )) ),
												  buildListProperty( "row3", 
																	 buildProperty( "x", value( 2, 0 )),
																	 buildProperty( "y", value( 2, 1 )),
																	 buildProperty( "z", value( 2, 2 )),
																	 buildProperty( "w", value( 2, 3 )) ),
												  buildListProperty( "row4", 
																	 buildProperty( "x", value( 3, 0 )),
																	 buildProperty( "y", value( 3, 1 )),
																	 buildProperty( "z", value( 3, 2 )),
																	 buildProperty( "w", value( 3, 3 )) ) );
					}
			};

			template<>
			struct PropertyBuilder< std::vector< bool >::reference, false >
			{
				static PropertyPtr build( const std::string& name, std::vector< bool >::reference value )
					{
						return BoolReferenceProperty::create( name, value );
					}
			};
		}

		template< class T >
		PropertyPtr buildProperty( const std::string& name, T& value )
		{
			return impl::PropertyBuilder< T, false >::build( name, value );
		}

		template< class T >
		PropertyPtr buildProperty( const std::string& name, const T& value )
		{
			return impl::PropertyBuilder< T, true >::build( name, value );
		}

		PropertyPtr buildColorProperty( const std::string& name, Eigen::Vector3f& value );
		PropertyPtr buildColorProperty( const std::string& name, Eigen::Vector4f& value );

		template< class T, class Constructor >
		PropertyPtr buildConstructorProperty( const std::string& name, T& value, Constructor c = Constructor() )
		{
			return SimpleConstructorProperty< T, Constructor >::create( name, value, c );
		}
	}
}
#endif
