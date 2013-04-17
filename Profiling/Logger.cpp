/*
   -----------------------------------------------------------------------------
   |                 R T R T    P R O F I L I N G   L I B R A R Y                 |
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
#include "Logger.hpp"
#include "Listener.hpp"

using std::string;
using std::vector;
using namespace rtrt::logging;

Logger::Logger(): _listeners()
{
}

void Logger::log( const string& stream, const string& message )
{
  _log( stream, message, vector< LogValue >() );
}

void Logger::_log( const string& stream, const string& message, const vector< LogValue >& values )
{
  Timestamp current( Timestamp::current() );

  BOOST_FOREACH( Listener* listener, _listeners )
  {
    listener->listen( Log( stream, message, values, current ) );
  }
}

void Logger::operator+=( Listener& listener )
{
  _listeners.insert( &listener );
} 

void Logger::operator-=( Listener& listener )
{
  _listeners.erase( &listener );
}
