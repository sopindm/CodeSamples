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

#ifndef RTRT_LOGGING_LOGGING_HPP
#define RTRT_LOGGING_LOGGING_HPP

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include "StreamListener.hpp"
#include "Logger.hpp"

namespace rtrt
{
  namespace logging
  {
    class Logging
    {
    public:
      static Logger& logger();

      static void begin( const std::string& filename );
      static void begin( const boost::shared_ptr< std::ostream >& stream );

      static void end();

    private:
      static Logger _logger;
      static boost::scoped_ptr< StreamListener > _listener;

    };
  }
}
#endif
