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

#ifndef RTRT_SCENEGRAPH_CAMERA_HPP
#define RTRT_SCENEGRAPH_CAMERA_HPP

#include <BaseCamera.hpp>
#include "CameraStash.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    class Camera;
    typedef boost::shared_ptr< Camera > CameraPtr;

    /*! A scenegraph's camera class */
    class Camera : public Stashable< CameraStash >
    {
    public:
      Camera();
      /*! Constructs camera using 3d-transfromation */
      Camera( const Eigen::Transform3f& transform );
      /*! Constructs camera using existing object */
      Camera( const BaseCameraPtr& baseCamera );

      /*! Creates default camera */
      static CameraPtr create();
      /*! Creates camera using 3d-transformation */
      static CameraPtr create( const Eigen::Transform3f& transform );
      /*! Creates camera using existing object */
      static CameraPtr create( const BaseCameraPtr& camera );

      /*! Sets used BaseCamera object */
      void setCamera( const BaseCameraPtr& camere );

      /*! Sets camera to use local coordinates space */
      void setLocal();
      /*! Sets camera to use world coordinates space */
      void setWorld();

      /*! A 3d-transformation of camera */
      Eigen::Transform3f transform() const;
      /*! Sets camera's transformation */
      void setTransform( const Eigen::Transform3f& transform );
      
      /*! Camera's position */
      Eigen::Vector3f position() const;
      /*! Sets camera position */
      void setPosition( const Eigen::Vector3f& position );

      /*! Rotates camera using specified angle */
      void rotate( const Eigen::Vector3f& angle );
      /*! Translates camera using specified offset */
      void translate( const Eigen::Vector3f& translation );

      /*! Camera's view vector */
      Eigen::Vector3f view() const;
      /*! Camera's up vector */
      Eigen::Vector3f up() const;
      /*! Camera's right vector */
      Eigen::Vector3f right() const;

      void update( double time );

      /*! Stashes camera */
      void stash( ScenegraphCameraPtr& camera ) const;

      /*! Renders scene from view of camera */
      void renderGL( const GLRender& render ) const;
      void generateRays( rtrt::cuda::DeviceBuffer< float3 >& origins, 
			 rtrt::cuda::DeviceBuffer< float3 >& directions,
			 const rtrt::cuda::DeviceBuffer< int >& baseIndices,
			 rtrt::cuda::DeviceBuffer< int >& screenIndices,
			 bool randomize, 
			 rtrt::cuda::DeviceBuffer< char >& states,
			 int raysPerPixel,
			 size_t raysCount ) const;

      std::string typeName() const;
      property::ListPropertyPtr properties();

    private:
      Camera( const Camera& );
      void operator=( const Camera& );

      void lookAt( const Eigen::Vector3f& eye, const Eigen::Vector3f& point, const Eigen::Vector3f& up ) const;
      void setupGL() const;

      BaseCameraPtr _baseCamera;

    };


  }
}
#endif
