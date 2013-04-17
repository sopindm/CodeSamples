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

#ifndef RTRT_CONVERT_MIXEDSCENEOSTREAM_HPP
#define RTRT_CONVERT_MIXEDSCENEOSTREAM_HPP

#include "XmlSceneOStream.hpp"
#include "TableSceneOStream.hpp"

namespace rtrt
{
  namespace convert
  {
    /*! Output stream writes file to mixed xml/binary format */
    class MixedSceneOStream : public XmlSceneOStream
    {
    public:
      /*! Creates mixed stream using specified ostream objects
       *  for xml and binary data 
       */
      MixedSceneOStream( std::tr1::shared_ptr< std::ostream > baseStream,
			 std::tr1::shared_ptr< std::ostream > binaryStream );
    
      bool writeRawData( void* data, size_t size );
    
      /*! Writing stream data to ostream */
      void flush();

    private:
    
      TableSceneOStreamPtr _tableStream;
      std::tr1::shared_ptr< std::ostream > _baseStream;
    };
  }
}
#endif
