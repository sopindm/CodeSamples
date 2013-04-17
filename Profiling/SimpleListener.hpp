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

#ifndef RTRT_LOGGING_SIMPLE_LISTENER_HPP
#define RTRT_LOGGING_SIMPLE_LISTENER_HPP

#include "Listener.hpp"

namespace rtrt
{
  namespace logging
  {
    namespace simpleListener
    {
      class Logs
      {
      public:
	Logs() {};

	size_t size() const { return _logs.size(); };
	const Log& operator()( size_t index ) const { return _logs[ index ]; };

	void operator+=( const Log& log ) { _logs.push_back( log ); };

      private:
	Logs( const Logs& );
	void operator=( const Logs& logs );

	std::vector< Log > _logs;

      };
    }
     
    class SimpleListener: public Listener
    {
    public:
      SimpleListener() {};

      const simpleListener::Logs& logs() const { return _logs; };
      const Log& log( size_t index ) const { return _logs( index ); };

      void listen( const Log& log ){ _logs += log; };

    private:
      SimpleListener( const SimpleListener& );
      void operator=( const SimpleListener& );

      simpleListener::Logs _logs;

    };
  }
}
#endif
