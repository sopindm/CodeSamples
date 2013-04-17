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

#include "PropertyVisitor.hpp"
#include "VectorProperty.hpp"
#include "property_uuid.uuid.h"

using namespace rtrt::property;

VectorProperty::VectorProperty( const std::string& name ): Property( name )
{
}

void VectorProperty::accept( PropertyVisitor& visitor )
{
  visitor.visit( *this );
}

void* VectorProperty::rawData()
{
  return NULL;
}

uint32_t VectorProperty::elementSize() const
{
  return 0;
}

const unsigned char* VectorProperty::type_uuid() const
{
  return property_uuid;
}
