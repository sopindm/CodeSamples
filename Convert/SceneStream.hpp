/*
   -----------------------------------------------------------------------------
   |                 R T R T    C O N V E R T   L I B R A R Y                 |
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

#ifndef RTRT_CONVERT_SCENESTREAM_HPP
#define RTRT_CONVERT_SCENESTREAM_HPP

#include <stdint.h>
#include <boost/tr1/memory.hpp>
#include <Core/Types.hpp>

namespace rtrt
{
  namespace convert
  {

    /*! An abstract stream working with scene objects. */
    class SceneStream
    {
    public:
      SceneStream();
      virtual ~SceneStream() {};

      /*! Enters section with specified name */
      virtual void enterSection( const std::string& name ) = 0;
      /*! Leaves current section */
      virtual void leaveSection() = 0;

      /*! Checks that stream is correct */
      bool correct();
      /*! A log with registred errors. */
      const std::string& errorLog() const;

    protected:
      /*! Adds new error message to log */
      void pushError( const std::string& error );

    private:
      void processColumn( float* v0, float* v1, float* v2, float* v3 );

      std::string _errorLog;
    };

    class SceneIStream : public SceneStream
    {
    public:
      SceneIStream();

      /*! Processing single size type value */
      virtual void read( uint32_t* value, const std::string& fieldName ) =0;
      /*! Processing single int type value */
      virtual void read( int* value, const std::string& fieldName ) = 0;
      /*! Processing single float value */
      virtual void read( float* value, const std::string& fieldName ) =0;
      /*! Processing string value */
      virtual void read( std::string* string, const std::string& name ) =0;

      virtual bool readRawData( void* data, size_t size ) { return false; };

    };

    class SceneOStream : public SceneStream
    {
    public:
      SceneOStream();

      /*! Processing single size type value */
      virtual void write( uint32_t value, const std::string& fieldName ) =0;
      /*! Processing single int type value */
      virtual void write( int value, const std::string& fieldName ) = 0;
      /*! Processing single float value */
      virtual void write( float value, const std::string& fieldName ) =0;
      /*! Processing string value */
      virtual void write( const std::string& string, const std::string& name ) =0;

      virtual bool writeRawData( void* data, size_t size ) { return false; };

      virtual void flush() {};

    };

    typedef std::tr1::shared_ptr< SceneIStream > SceneIStreamPtr;
    typedef std::tr1::shared_ptr< SceneOStream > SceneOStreamPtr;
  }
}
#endif
