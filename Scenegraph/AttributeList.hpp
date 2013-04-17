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

#ifndef RTRT_SCENEGRAPH_ATTRIBUTELIST_HPP
#define RTRT_SCENEGRAPH_ATTRIBUTELIST_HPP

#include <map>
#include "Attribute.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    /*! A list of scenegraph's attributes */
    class AttributeList
    {
    public:
      AttributeList();
      /*! Constructs new list as a copy of existing one */
      AttributeList( const AttributeList& list );

      /*! Copies attributes list */
      void operator=( const AttributeList& list );

      /*! Gets attribute */
      AttributePtr attribute( const std::string& name ) const;
      /*! Sets attribute */
      void setAttribute( const std::string& name,  const Attribute& value );

      /*! Merges two lists */
      AttributeList merge( const AttributeList& list ) const;

      /*! Returns all atributes presented in object, but missing in list */
      AttributeList delta( const AttributeList& list ) const;
      /*! Sets all list's attributes new */
      void touch( const AttributeList& list );

    private:
      void mergeAttribute( const std::string& name, const Attribute& value );
      void touchAttribute( const std::string& name, const Attribute& oldValue );

      void copyList( const AttributeList& list );

      typedef std::map< std::string, AttributePtr > AttributeMap;
      AttributeMap _attrs;

    };
  }
}

namespace rtrt
{
  namespace scenegraph
  {
    template< class T >
    boost::shared_ptr< T > getAttributeValue( const char* name, const AttributeList& list )
    {
      boost::shared_ptr< T > value = boost::dynamic_pointer_cast< T >( list.attribute( name ));
      if( value == NULL )
	value.reset( new T() );
      
      return value;
    }
  }
}

#define __SCENEGRAPH_ATTRIBUTE_TYPE( name ) rtrt::scenegraph::name##Attribute
#define SCENEGRAPH_ATTRIBUTE( name, list ) rtrt::scenegraph::getAttributeValue                               \
  <__SCENEGRAPH_ATTRIBUTE_TYPE( name ) >( #name, list )

#define SCENEGRAPH_SET_ATTRIBUTE( value, name, list )                                      \
  (list.setAttribute( #name,						                   \
		      static_cast< const rtrt::scenegraph::name##Attribute& >( value )))


#endif
