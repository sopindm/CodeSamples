/*
   -----------------------------------------------------------------------------
   |                 R T R T    C O N V E R T   L I B R A R Y                 |
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

#ifndef RTRT_CONVERT_PROPERTYREADER_HPP
#define RTRT_CONVERT_PROPERTYREADER_HPP

#include <Property/PropertyVisitor.hpp>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    class PropertyReader: public property::PropertyVisitor
    {
    public:
      PropertyReader( const SceneIStreamPtr& stream = SceneIStreamPtr() );

      SceneIStreamPtr stream();
      void setStream( const SceneIStreamPtr& stream );

      void read( const property::PropertyPtr& property );

      void visit( property::BoolProperty& property );
      void visit( property::IntProperty& property );
      void visit( property::UIntProperty& property );
      void visit( property::FloatProperty& property );
      void visit( property::StringProperty& property );

      void visit( property::VectorProperty& property );
      void visit( property::ListProperty& property );
      void visit( property::ConstructorProperty& property );

    private:
      class ElementReader;

      PropertyReader( const PropertyReader& reader );
      void operator=( const PropertyReader& reader );

      template< class T >
      void doVisit( property::TypedProperty< T >& property );

      template< class T >
      void doVisit( property::TypedProperty< T >& property, const std::string& name );

      template< class T >
      void visitElement( property::TypedProperty< T >& property );

      SceneIStreamPtr _stream;

    };
  }
}
#endif
