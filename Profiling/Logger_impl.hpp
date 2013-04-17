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

namespace rtrt
{
  namespace logging
  {
    template< class T >
    class Logger::Store
    {
    public:
      static void store( std::vector< LogValue >& values, T value )
      {
	values.push_back( value );
      }
    };

    template<>
    class Logger::Store< double >
    {
    public:
      static void store( std::vector< LogValue >& values, double value )
      {
	values.push_back( static_cast< float >( value ) );
      }
    };

    template<>
    class Logger::Store< const char* >
    {
    public:
      static void store( std::vector< LogValue >& values, const char* value )
      {
	values.push_back( static_cast< std::string >( value ) );
      }
    };

    template< class T >
    void Logger::log( const std::string& stream, const std::string& message, T value )
    {
      std::vector< LogValue > values;
      Store< T >::store( values, value );
      
      _log( stream, message, values );
    }

    template< class T1, class T2 >
    void Logger::log( const std::string& stream, const std::string& message, T1 value1, T2 value2 )
    {
      std::vector< LogValue > values;
      Store< T1 >::store( values, value1 );
      Store< T2 >::store( values, value2 );
      
      _log( stream, message, values );
    }

    template< class T1, class T2, class T3 >
    void Logger::log( const std::string& stream, const std::string& message, T1 value1, T2 value2, T3 value3 )
    {
      std::vector< LogValue > values;
      Store< T1 >::store( values, value1 );
      Store< T2 >::store( values, value2 );
      Store< T3 >::store( values, value3 );
      
      _log( stream, message, values );
    }

    template< class T1, class T2, class T3, class T4 >
    void Logger::log( const std::string& stream, const std::string& message, 
		      T1 value1, T2 value2, T3 value3, T4 value4 )
    {
      std::vector< LogValue > values;
      Store< T1 >::store( values, value1 );
      Store< T2 >::store( values, value2 );
      Store< T3 >::store( values, value3 );
      Store< T4 >::store( values, value4 );
      
      _log( stream, message, values );
    }

    template< class T1, class T2, class T3, class T4, class T5 >
    void Logger::log( const std::string& stream, const std::string& message, 
		      T1 value1, T2 value2, T3 value3, T4 value4, T5 value5 )
    {
      std::vector< LogValue > values;
      Store< T1 >::store( values, value1 );
      Store< T2 >::store( values, value2 );
      Store< T3 >::store( values, value3 );
      Store< T4 >::store( values, value4 );
      Store< T5 >::store( values, value5 );
      
      _log( stream, message, values );
    }
  }
}
