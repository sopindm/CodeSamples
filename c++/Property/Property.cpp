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

#include <boost/foreach.hpp>
#include "Property.hpp"

using namespace rtrt::property;

Property::Property( const std::string& name ): _name( name ),
					       _handlers( new Handlers() )
{};

const std::string& Property::name() const
{
  return _name;
}

void Property::setName( const std::string& name )
{
  _name = name;
  signalUpdate();
}

void Property::connect( const UpdateHandlerPtr& handler )
{
  _handlers->handlers.insert( handler );
}

void Property::disconnect( const UpdateHandlerPtr& handler )
{
  _handlers->handlers.erase( handler );
}

void Property::signalUpdate() const
{
  _handlers->signal();
}

void Property::setParent( const Property& property )
{
  _handlers->parentHandlers = property._handlers;
}

void Property::Handlers::signal() const
{
  BOOST_FOREACH( UpdateHandlerPtr handler, handlers )
    handler->update();

  if( parentHandlers != NULL )
    parentHandlers->signal();
}


