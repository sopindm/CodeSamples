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

#ifndef RTRT_CONVERT_BINARYSCENEOSTREAM_HPP
#define RTRT_CONVERT_BINARYSCENEOSTREAM_HPP

#include <ostream>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! An output scene stream using plain text */
    class BinarySceneOStream : public SceneOStream
    {
    public:

      /*! Creates new binary ostream with specified ostream object */
      explicit BinarySceneOStream( std::tr1::shared_ptr< std::ostream > stream );

      void write( int value, const std::string& fieldName );
      void write( uint32_t value, const std::string& fieldName );
      void write( float value, const std::string& fieldName );
      void write( const std::string& string, const std::string& name );

      bool writeRawData( void* data, size_t size );

      void enterSection( const std::string& name );
      void leaveSection();

      bool correct();

    private:

      BinarySceneOStream();
      BinarySceneOStream( const BinarySceneOStream& );

      BinarySceneOStream& operator=( const BinarySceneOStream& );

      std::tr1::shared_ptr< std::ostream > _stream;
      int _indentation;
    };
  }
}
#endif
