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

#include <boost/assign/std/vector.hpp>
#include "PropertyBuilder.hpp"
#include "ListProperty.hpp"

using std::string;
using namespace boost::assign;
using namespace rtrt::property;

ListPropertyPtr rtrt::property::buildListProperty( const string& name )
{
  std::vector< PropertyPtr > props;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, const PropertyPtr& p1 )
{
  std::vector< PropertyPtr > props;
  props += p1;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, const PropertyPtr& p1, const PropertyPtr& p2 )
{
  std::vector< PropertyPtr > props;
  props += p1, p2;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3 )
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4 )
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5 )
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5, 
						   const PropertyPtr& p6 )
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5, p6;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5, 
						   const PropertyPtr& p6,
						   const PropertyPtr& p7 )
					       
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5, p6, p7;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5, 
						   const PropertyPtr& p6,
						   const PropertyPtr& p7,
						   const PropertyPtr& p8 )
					       
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5, p6, p7, p8;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5, 
						   const PropertyPtr& p6,
						   const PropertyPtr& p7,
						   const PropertyPtr& p8,
						   const PropertyPtr& p9 )
					       
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5, p6, p7, p8, p9;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5, 
						   const PropertyPtr& p6,
						   const PropertyPtr& p7,
						   const PropertyPtr& p8,
						   const PropertyPtr& p9,
						   const PropertyPtr& p10 )

					       
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5, 
						   const PropertyPtr& p6,
						   const PropertyPtr& p7,
						   const PropertyPtr& p8,
						   const PropertyPtr& p9,
						   const PropertyPtr& p10,
						   const PropertyPtr& p11 )

					       
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11;

  return ListProperty::create( name, props );
}

ListPropertyPtr rtrt::property::buildListProperty( const string& name, 
						   const PropertyPtr& p1, 
						   const PropertyPtr& p2, 
						   const PropertyPtr& p3, 
						   const PropertyPtr& p4, 
						   const PropertyPtr& p5, 
						   const PropertyPtr& p6,
						   const PropertyPtr& p7,
						   const PropertyPtr& p8,
						   const PropertyPtr& p9,
						   const PropertyPtr& p10,
						   const PropertyPtr& p11,
						   const PropertyPtr& p12 )

					       
{
  std::vector< PropertyPtr > props;
  props += p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12;

  return ListProperty::create( name, props );
}

PropertyPtr rtrt::property::buildColorProperty( const std::string& name, Eigen::Vector3f& value )
{
  std::vector< PropertyPtr > props;
  props += buildProperty( "r", value.x() ), 
    buildProperty( "g", value.y() ),
    buildProperty( "b", value.z() );

  return ColorProperty::create( name, props );
}

PropertyPtr rtrt::property::buildColorProperty( const std::string& name, Eigen::Vector4f& value )
{
  std::vector< PropertyPtr > props;
  props += buildProperty( "r", value.x() ), 
    buildProperty( "g", value.y() ),
    buildProperty( "b", value.z() ),
    buildProperty( "a", value.w() );

  return ColorProperty::create( name, props );
}

