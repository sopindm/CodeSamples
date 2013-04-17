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

#ifndef RTRT_SCENEGRAPH_TIME_HPP
#define RTRT_SCENEGRAPH_TIME_HPP

#include <cstddef>

namespace rtrt
{
  namespace scenegraph
  {
    /*! Inner scenegraph time. Used to check updates. */
    class Time
    {
    public:
      /*! Creates new time object */
      Time();
      /*! Creates new time object from value */
      explicit Time( size_t value );
      /*! Creates new time object from existing one */ 
      Time( const Time& time );
      
      /*! Copies time value from existing object. */
      void operator=( const Time& time );
      /*! Increments time value */
      void operator++();
      /*! Increments time value */
      void operator++( int );

      /*! Checks time objects equality */
      bool operator==( const Time& time ) const;
      /*! Checks less relation between objects */
      bool operator<( const Time& time ) const;
      /*! Checks less or equal relation between objects */
      bool operator<=( const Time& time ) const;
      /*! Checks more relation between objects */
      bool operator>( const Time& time ) const;
      /*! Checks more or equal relation between objects */
      bool operator>=( const Time& time ) const;

      /*! Sets time object newest */
      void setNew();
      /*! Checks that time object is newest */
      bool isNewest() const;

      /*! Value of oldest objects */
      static const size_t oldest = 0;
      /*! Start of low time interval */
      static const size_t lowest = 1;
      /*! Start of middle time interval */
      static const size_t low = 1000000;
      /*! Start of high time interval */
      static const size_t high = 2000000;
      /*! End of high time interval */
      static const size_t highest = 3000000;
      /*! Value of newest time */
      static const size_t newest = 3000001;

    private:
      void testOldest( const Time& newTime ) const;

      bool isOldest() const;
      bool isLow() const;
      bool isMiddle() const;
      bool isHigh() const;

      bool isInHighPart() const;
      
      mutable size_t _value;
    };

    /*! Base class of all timed scenegraph's objects */
    class Timed
    {
    public:
      /*! Constructs using oldest time value */
      Timed(): _time() {};

      /*! Constucts new object as a copy of existing one */
      Timed( const Time& time ): _time( time ) {};
      
      /*! Checks that object is old in specified time */
      bool isOld( const Time& time ) const { return _time < time; };
      /*! Checks that object is new in specified time */
      bool isNew( const Time& time ) const { return _time >= time; };

      /*! Inner scenegraph method. Should not be used in user code */

      void resolveNewest( const Time& timeSource ) { if( _time.isNewest() ) _time = timeSource; };
      /*! Sets object new */
      void setNew() { _time.setNew(); };

    protected:
      /*! An object's update time */
      const Time& time() const { return _time; };

    private:
      Time _time;

    };
  }
}
#endif
