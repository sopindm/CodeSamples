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

#ifndef RTRT_PROPERTY_PROPERTYVISITOR_HPP
#define RTRT_PROPERTY_PROPERTYVISITOR_HPP

#include "Property.hpp"
#include "AtomProperty.hpp"
#include "ListProperty.hpp"
#include "VectorProperty.hpp"
#include "ConstructorProperty.hpp"

namespace rtrt
{
  namespace property
  {
    class PropertyVisitor
    {
    public:
      virtual void visit( BoolProperty& property ) = 0;
      virtual void visit( IntProperty& property ) = 0;
      virtual void visit( UIntProperty& property ) = 0;
      virtual void visit( FloatProperty& property ) = 0;
      virtual void visit( StringProperty& property ) = 0;

      virtual void visit( ListProperty& property ) = 0;
      virtual void visit( VectorProperty& property ) = 0;
      virtual void visit( ConstructorProperty& property ) = 0;
    };
  }
}
#endif
