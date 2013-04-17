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

#ifndef RTRT_SCENEGRAPH_CAMERASTASH_HPP
#define RTRT_SCENEGRAPH_CAMERASTASH_HPP

#include <Eigen/Core>
#include <Core/Types.hpp>
#include <BaseCamera.hpp>
#include <Cuda/Buffer.hpp>

#include "BaseStash.hpp"
#include "IndexedVector.hpp"
#include "Rendering/Viewport.hpp"
#include <Cuda/Random.h>

namespace rtrt
{
  namespace scenegraph
  {
    namespace cameraStash
    {
      class StashInterface;
    }

    typedef boost::shared_ptr< cameraStash::StashInterface > ScenegraphCameraPtr;

    class GLRender;

    namespace cameraStash
    {
      typedef std::vector< Eigen::Transform3f, Eigen::aligned_allocator< Eigen::Transform3f > > TransformArray;

      /*! A stashing interface for camers */
      class StashInterface : public BaseStashInterface, public BaseCamera
      {
      public:
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW
	StashInterface();

	/*! Renders scene from view of camera */
	virtual void renderGL( const GLRender& render ) const = 0;
	virtual void generateRays( rtrt::cuda::DeviceBuffer< float3 >& origins, 
				   rtrt::cuda::DeviceBuffer< float3 >& directions,
				   const rtrt::cuda::DeviceBuffer< int >& baseIndices,
				   rtrt::cuda::DeviceBuffer< int >& screenIndices,
				   bool randomize,
				   rtrt::cuda::DeviceBuffer< char >& states,
				   int raysPerPixel,
				   size_t raysCount ) const = 0;

	/*! Stashes camera */
	virtual void stash( ScenegraphCameraPtr& camera ) const = 0;
	
	/*! Transformation of camera node in scenegraph */
	const Eigen::Transform3f& baseTransform() const;
	/*! Set's transformation of camera in scenegraph */
	void setBaseTransform( const Eigen::Transform3f& transform );

	/*! Camera's viewport */
	const ViewportPtr& viewport() const;
	/*! Sets camera viewport */
	void setViewport( const ViewportPtr& viewport );

      private:
	Eigen::Transform3f _baseTransform;
	ViewportPtr _viewport;

      };

      typedef std::vector< ScenegraphCameraPtr > CameraArray;
    }

    /*! Stash class for cameras */
    class CameraStash : public Stash< cameraStash::StashInterface >
    {
    public:
      /*! Stashes name */
      static std::string stashName() { return "CameraStash"; };
      CameraStash();
      
      /*! Stashes cameras array */
      const cameraStash::CameraArray& cameras() const;
      uint32_t cameraID( const ScenegraphCameraPtr& camera ) const;

      const unsigned char* type_uuid() const;
      std::string typeName() const { return stashName(); };

    protected:
      void doAddObject( BaseStashable* , size_t id );
      void doRemoveObject( BaseStashable*, size_t id );
      void doAllocateObject( BaseStashable* object, size_t id );
      void doFreeObject( BaseStashable* object, size_t id );
      void doWriteObject( BaseStashable* object, size_t id, const AttributeList& attributes, const Time& time );

    private:	
      CameraStash( const CameraStash& stash );
      const CameraStash& operator=( const CameraStash& stash );

      IndexedVector< cameraStash::CameraArray > _cameras;

    };
    typedef boost::shared_ptr< CameraStash > CameraStashPtr;
    
  }
}
#endif
