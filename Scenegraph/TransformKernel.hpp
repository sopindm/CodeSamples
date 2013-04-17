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

#ifndef RTRT_SCENEGRAPH_TRANSFORM_KERNEL_HPP
#define RTRT_SCENEGRAPH_TRANSFORM_KERNEL_HPP

#include "Kernel.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    class TransformKernel;
    typedef boost::shared_ptr< TransformKernel > TransformKernelPtr;

    /*! A kernel representing transformation */
    class TransformKernel: public Kernel
    {
    public:
      /*! Creates kernel with identity transform */
      TransformKernel();

      /*! Creates new kernel using specified translation */
      static TransformKernelPtr createTranslation( float dx, float dy, float dz );
      /*! Creates new kernel using specified translation */
      static TransformKernelPtr createTranslation( const Eigen::Vector3f& value );
      /*! Creates new kernel using specified rotation */
      static TransformKernelPtr createRotation( float ax, float ay, float az );
      /*! Creates new kernel using specified rotation */
      static TransformKernelPtr createRotation( const Eigen::Vector3f& angle );
      /*! Creates new kernel using specified scaling */
      static TransformKernelPtr createScaling( float sx, float sy, float sz );
      /*! Creates new kernel using specified scaling */
      static TransformKernelPtr createScaling( const Eigen::Vector3f& scale );

      /*! Rotates using specified angles.
       *  Order: Z, Y, X
       */
      void rotate( float angleX, float angleY, float angleZ );
      /*! Rotates using specified angles.
       *  Order: Z, Y, X
       */
      void rotate( const Eigen::Vector3f& angles );

      /*! Translates using specified offset */
      void translate( float dx, float dy, float dz );
      /*! Translates using specified offset */
      void translate( const Eigen::Vector3f& translation );

      /*! Scales using specified coefficients */
      void scale( float sx, float sy, float sz );
      /*! Scales using specified coefficients */

      void scale( const Eigen::Vector3f& scale );

      void update( double time );

      std::string typeName() const;
      property::ListPropertyPtr properties();

      typedef boost::shared_ptr< Eigen::Transform3f > TransformPtr;
      TransformPtr transform() const;

    private:
      Eigen::Vector3f _translation;
      Eigen::Vector3f _rotation;
      Eigen::Vector3f _scale;

    };

  }
}
#endif
