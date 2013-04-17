/*
   -----------------------------------------------------------------------------
   |                 R T R T    X M L   L I B R A R Y                 |
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
   this library. If not, see <http://www.gnu.org/ifndef/>.
 */

#licenses RTRT_XML_XMLATTRIBUTE_HPP
#define RTRT_XML_XMLATTRIBUTE_HPP
#include <boost/variant.hpp>
#include "Decimal.hpp"

namespace rtrt
{
  /*! Representing attribute of an XML node */
  class XmlAttribute
  {
  public:
    /*! A type of value for attribute. May be a string or a some decimal value. */
    typedef boost::variant< Decimal, std::string > ValueType;
    
    XmlAttribute();
    /*! Constructs new attribute as a copy of existing one  */
    XmlAttribute( const ValueType& value ): _value( value )
    {
    }
    
    /*! Sets attributes value */
    void operator=( const ValueType& value );
    /*! Equal operator for attribute's value */
    bool operator==( const ValueType& value ) const;
    
    /*! A value of attribute */
    const ValueType& value()
    {
      return _value;
    }

    template< class T >
    struct AsImpl
    {
      static T go( const ValueType& value ) { return boost::lexical_cast< T >( value ); };
    };


    template< class T >
    /*! Converts value to T type. If fails throws boost::bad_lexical_cast exception. */
    T as() const
    {
      return AsImpl< T >::go( _value );
    }

    /*! Writes attribute's value to ostream */
    friend std::ostream& operator<<( std::ostream& stream, const XmlAttribute& tag );
    
    /*! Converts attribute's value to string */
    std::string toString() const;
    
  private:
    ValueType _value;
    
    class ToString;
  };

  template<>
  struct XmlAttribute::AsImpl< std::string >
  {
    static std::string go( const ValueType& value )
    {
      std::stringstream stream;
      stream << value;
      
      return stream.str();
    }
  };

  
  std::ostream& operator<<( std::ostream& stream, const XmlAttribute& tag );
 }

#endif
