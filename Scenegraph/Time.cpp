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

#include "Time.hpp"

using namespace rtrt::scenegraph;

Time::Time(): _value( oldest )
{
}

Time::Time( size_t value ): _value( value )
{
}

Time::Time( const Time& time ): _value( time._value )
{
}

void Time::operator=( const Time& time )
{
  _value = time._value;
}

void Time::operator++()
{
  if( isOldest() )
    return;
  if( isNewest() )
    return;

  if( _value == highest )
  {
    _value = lowest;
    return;
  }

  _value++;
}

void Time::operator++( int )
{
  ++(*this);
}

bool Time::operator==( const Time& time ) const
{
  return _value == time._value;
}

bool Time::operator<( const Time& time ) const
{
  if( _value == oldest )
    return true;

  if( isLow() && time.isHigh() )
  {
    time.testOldest( *this );
    return false;
  }

  if( isHigh() && time.isLow() )
  {
    testOldest( time );
    return true;
  }

  if( _value < time._value )
  {
    testOldest( time );
    return true;
  }

  time.testOldest( *this );
  return false;
}

bool Time::operator<=( const Time& time ) const
{
  return !(*this > time);
}

bool Time::operator>( const Time& time ) const
{
  return time < *this;
}

bool Time::operator>=( const Time& time ) const
{
  return !(*this < time);
}

void Time::setNew()
{
  _value = newest;
}

bool Time::isOldest() const
{
  return _value == oldest;
}

bool Time::isNewest() const
{
  return _value == newest;
}

bool Time::isLow() const
{
  return _value >= lowest && _value < low;
}

bool Time::isMiddle() const
{
  return _value >= low && _value < high;
}

bool Time::isHigh() const
{
  return _value >= high && _value <= highest;
}

bool Time::isInHighPart() const
{
  if( isLow() )
    return _value > low / 2;

  if( isMiddle() )
    return _value > (low + (high - low) / 2);

  if( isHigh() )
    return _value > (high + (highest - high) / 2);

  return false;
}

void Time::testOldest( const Time& time ) const
{
  if( isLow() && time.isMiddle() && time.isInHighPart() )
    _value = oldest;

  if( isMiddle() && time.isHigh() && time.isInHighPart() )
    _value = oldest;

  if( isHigh() && time.isLow() && time.isInHighPart() )
    _value = oldest;
}
