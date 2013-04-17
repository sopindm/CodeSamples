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

#ifndef RTRT_CONVERT_BINARYSCENEISTREAM_HPP
#define RTRT_CONVERT_BINARYSCENEISTREAM_HPP

#include <istream>
#include "SceneStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! An input scene stream using binary data */
    class BinarySceneIStream : public SceneIStream
    {
    public:
    
      /*! Creates new binary istream with specified istream object */
      explicit BinarySceneIStream( std::tr1::shared_ptr< std::istream > stream );
    
      void read( int* value, const std::string& fieldName );
      void read( uint32_t* value, const std::string& fieldName );
      void read( float* value, const std::string& fieldName );
      void read( std::string* string, const std::string& name );
    
      bool readRawData( void* data, size_t size );
    
      void enterSection( const std::string& name );
      void leaveSection();
    
      bool correct();
    
    private:
    
      BinarySceneIStream();
      BinarySceneIStream( const BinarySceneIStream& );
    
      BinarySceneIStream& operator=( const BinarySceneIStream& );
    
      std::tr1::shared_ptr< std::istream > _stream;
      int _indentation;
      bool _correct;
    };
  }
}
#endif
