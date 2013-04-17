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

#ifndef RTRT_SCENEGRAPH_ATTRIBUTE_HPP
#define RTRT_SCENEGRAPH_ATTRIBUTE_HPP

#include <boost/shared_ptr.hpp>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include "Time.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    class Attribute;
    typedef boost::shared_ptr< Attribute > AttributePtr;

    /*! A scenegraph's attribute. */
    class Attribute : public Timed
    {
    public:
      EIGEN_MAKE_ALIGNED_OPERATOR_NEW
      /*! Constructs attribute using specified time */
      Attribute( const Time& time );
      /*! Copies attribute using specified time */
      Attribute( const Attribute& attribute, const Time& time );
      virtual ~Attribute();
      
      /*! Merges value of attribute with new one. */
      virtual void merge( const Attribute& newAtrribute );
      /*! Clones attribute */
      virtual AttributePtr clone() const = 0;
      /*! Default value of attribute class */
      virtual AttributePtr defaultValue() const = 0;

    private:
      Attribute( const Attribute& attribute );
      void operator=( const Attribute& attribute );

    };

    /*! A 3d-transformation attribute */
    class TransformAttribute : public Attribute
    {
    public:
      /*! Constructs attribute using time */
      TransformAttribute( const Time& time = Time( Time::oldest ));
      /*! Constructs attribute using 3d-transformation matrix */
      TransformAttribute( const Eigen::Transform3f& transform, const Time& time = Time( Time::newest ));

      void merge( const Attribute& newAttribute );
      AttributePtr clone() const;
      AttributePtr defaultValue() const { return AttributePtr( new TransformAttribute() ); };

      /*! A value of attribute */
      const Eigen::Transform3f& value() const;
      /*! Sets attribute's value */
      void setValue( const Eigen::Transform3f& transform );

    private:
      TransformAttribute( const TransformAttribute& attribute );
      void operator=( const TransformAttribute& attribute );

      Eigen::Transform3f _transform;

    };
    typedef boost::shared_ptr< TransformAttribute > TransformAttributePtr;

    /*! A boolean visibility attribute */
    class VisibilityAttribute : public Attribute
    {
    public:
      /*! Constructs attribute using time */
      VisibilityAttribute( const Time& time = Time( Time::oldest ));
      /*! Constructs attribute using boolean visibility value */
      VisibilityAttribute( bool isVisible, const Time& time = Time( Time::newest ));

      void merge( const Attribute& newAttribute );
      AttributePtr clone() const;
      AttributePtr defaultValue() const { return AttributePtr( new VisibilityAttribute() ); };

      /*! An attribute's value */
      bool value() const;
      /*! Sets attribute's value */
      void setValue( bool value );

    private:
      VisibilityAttribute( const VisibilityAttribute& );
      void operator=( const VisibilityAttribute& );

      bool _isVisible;
    };
    typedef boost::shared_ptr< VisibilityAttribute > VisibilityAttributePtr;

  }
}
#endif
