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

#ifndef RTRT_SCENEGRAPH_KERNEL_HPP
#define RTRT_SCENEGRAPH_KERNEL_HPP

#include <boost/shared_ptr.hpp>
#include "AttributeList.hpp"
#include <Property/ListProperty.hpp>

namespace rtrt
{
  namespace scenegraph
  {
    class Node;
    class Scenegraph;

    /*! Class for node's kernels */
    class Kernel
    {
    public:
      Kernel(): _node( NULL ) {};
      virtual ~Kernel() {};

      /*! Updates kernel */
      virtual void update( double time ) {};

      /*! Assigns current node to kernel */
      void assignNode( Node* node ) { _node = node; };

      /*! Current kernel's node */
      Node* node() { return _node; };

      /*! Current kernel's node */
      const Node* node() const { return _node; };

      /*! Sets kernel new */
      void renew() { _time.setNew(); };

      virtual std::string typeName () const = 0;
      virtual property::ListPropertyPtr properties () = 0;

    protected:
      /*! Kernel's time value */
      const Time& time() const { return _time; };

    private:
      friend class Scenegraph;

      Kernel( const Kernel& kernel );
      void operator=( const Kernel& kernel );

      Node* _node;
      Time _time;

    };
    typedef boost::shared_ptr< Kernel > KernelPtr;
  }
}

#include "Node.hpp"

#define KERNEL_SET_ATTRIBUTE( value, name ) (node()->attributes().setAttribute(                  \
					       #name,	name##Attribute( value, Kernel::time() )))
#endif
