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

#include "Logging.hpp"

using std::string;
using std::ostream;
using boost::shared_ptr;
using boost::scoped_ptr;
using namespace rtrt::logging;

Logger Logging::_logger;
scoped_ptr< StreamListener > Logging::_listener;

Logger& Logging::logger() { return _logger; };

void Logging::begin( const string& filename )
{
  end();

  _listener.reset( new StreamListener( filename ));
  _logger += *_listener;
}

void Logging::begin( const shared_ptr< ostream >& stream )
{
  end();

  _listener.reset( new StreamListener( stream ));
  _logger += *_listener;
}

void Logging::end()
{
 if( _listener != NULL )
    _logger -= *_listener;
}
