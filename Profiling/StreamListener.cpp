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

#include <fstream>
#include <boost/foreach.hpp>
#include "StreamListener.hpp"

using boost::shared_ptr;
using std::string;
using std::ostream;
using std::ofstream;
using namespace rtrt::logging;

StreamListener::StreamListener( const string& filename ):
  _stream( new ofstream( filename.c_str() ))
{
}

StreamListener::StreamListener( const shared_ptr< ostream >& stream ):
  _stream( stream )
{
}

class StreamListener::ValuePrinter: public boost::static_visitor<>
{
public:
  ValuePrinter( std::ostream& stream ): _stream( stream ) {};

  template< class T >
  void operator()( T value )
  {
    _stream << value;
  }

  void operator()( bool value )
  {
    _stream << (value ? "true" : "false");
  }

  void operator()( const string& value )
  {
    _stream << StreamListener::quote( value );
  }

private:
  std::ostream& _stream;

};

void StreamListener::listen( const Log& log )
{
  *_stream << quote( log.stream() ) << ' ' 
	   << quote( log.message() ) << ' ';

  BOOST_FOREACH( const Logger::LogValue& value, log.values() )
  {
    ValuePrinter printer( *_stream );
    boost::apply_visitor( printer, value );

    *_stream << ' ';
  }

  *_stream << log.timestamp() << std::endl;
}

string StreamListener::quote( std::string str )
{
  for( size_t i=0; i<str.size(); i++ )
  {
    if( str[ i ] == '"' )
    {
      str.insert( i, "\\" );
      i++;
    }
    if( str[ i ] == '\\' )
    {
      str.insert( i, "\\" );
      i++;
    }
    if( str[ i ] == '\n' )
    {
      str.erase( i, 1 );
      str.insert( i, "\\n" );
      i++;
    }
  }

  return '"' + str + '"';
}
