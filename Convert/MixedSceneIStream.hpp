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

#ifndef RTRT_CONVERT_MIXEDSCENEISTREAM_HPP
#define RTRT_CONVERT_MIXEDSCENEISTREAM_HPP

#include "XmlSceneIStream.hpp"
#include "TableSceneIStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! A input stream reading from mixed xml/binary format. */
    class MixedSceneIStream : public XmlSceneIStream
    {
    public:
      /*! Constructs new stream using specified istreams for xml and binary data. */
      MixedSceneIStream( std::tr1::shared_ptr< std::istream > baseStream,
			 std::tr1::shared_ptr< std::istream > binaryStream );
      /*! Initializied stream */
      void init();
    
      /*! Is checking of node hash sum needed? */
      void setChecking( bool check );
      /*! Log of checking results */
      const std::string& checkLog() const;

      bool readRawData( void* data, size_t size );
    
    private:
      void checkSha();
    
      std::tr1::shared_ptr< std::istream > _baseStream;
      TableSceneIStreamPtr _tableStream;
    
      bool _checking;
      std::string _checkLog;
    };
  }
}
#endif
