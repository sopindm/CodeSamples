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

#ifndef BASE_PROPERTY_HAHDLER_HPP
#define BASE_PROPERTY_HAHDLER_HPP

#include "BasePropertyHandler.hpp"
#include <boost/shared_ptr.hpp>
#include <Property/Property.hpp>

namespace rtrt
{
  namespace scenegraph
  {
    class BaseStashInterface;

    class BasePropertyHandler: public property::UpdateHandler
    {
    public:
      BasePropertyHandler( BaseStashInterface* stashInterface ): _stashInterface( stashInterface )
      {
      }

      void update();

    private:
      BaseStashInterface* _stashInterface;

    };

    typedef boost::shared_ptr< BasePropertyHandler > BasePropertyHandlerPtr;
  }
}

#endif
