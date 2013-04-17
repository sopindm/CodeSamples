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

#ifndef RTRT_SCENEGRAPH_NODE_HPP
#define RTRT_SCENEGRAPH_NODE_HPP

#include <vector>
#include "Kernel.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    class Scenegraph;
    class SceneVisitor;

    class Node;
    typedef boost::shared_ptr< Node > NodePtr;

    /*! A node of a scenegraph */
    class Node
    {
    public:
      /*! Constructs new code */
      Node();
      /*! Construct code copying existing one */
      Node( const Node& node );
      /*! Desturcts node */
      ~Node();

      /*! Creates new node using specified sample and parent node */
      static NodePtr create( const NodePtr& node = NodePtr( new Node()), Node* parent = NULL );
      /*! Creates new node with specified kernel and parent */
      static NodePtr create( const KernelPtr& kernel, Node* parent = NULL );
      /*! Creates new node with specified kernel and parent */
      static NodePtr create( const KernelPtr& kernel, NodePtr parent );
      /*! Creates new empty node with specified parent */
      static NodePtr createWithParent( Node* parent );

      /*! Copies node data from existing one */
      void operator=( const Node& node );

      /*! Node's name */
      const std::string& name() const;
      /*! Sets node's name */
      void setName( const std::string& name );

      /*! Parent node */
      Node* parent() const;

      class NodeChildsProxy;
      /*! A node's childs */
      NodeChildsProxy childs();

      /*! Used scenegraph engine */
      Scenegraph* scenegraph() const;
      /*! Updates scenegraph */
      void update( double time );

      /*! Node's kernel */
      const KernelPtr& kernel() const;
      /*! Sets kernel */
      void setKernel( const KernelPtr& kernel );
      /*! Removes kernel. Node becomes empty */
      void removeKernel();
      /*! Removes kernels of all children */
      void removeChildrenKernels();

      /*! An attributes for node. */
      AttributeList& attributes();

      void clearChilds();
      void addChildSimple( NodePtr node );
    private:
      void makeScenegraph();

      void addChild( NodePtr node );
      void removeChild( NodePtr child );

      void addToScenegraph( Scenegraph* scenegraph );
      void removeFromScenegraph();
      

      void copyChilds( const Node& node );

      std::vector< NodePtr > _childs;
      Node* _parent;

      Scenegraph* _scenegraph;

      KernelPtr _kernel;

      AttributeList _attributes;

      std::string _name;
    };

    /*! A proxy class for node's childs */
    class Node::NodeChildsProxy
    {
    public:
      /*! Proxy class for insert comma separated values */
      class NodeCommaInserter
      {
      public:
	/*! Inserts node to parent's childs */
	NodeCommaInserter( Node& parent, const NodePtr& node );
	/*! Inserts one more node */
	NodeCommaInserter operator,( const NodePtr& node );

      private:
	Node& _parent;

      };

      /*! Adds some comma separated nodes to parent */
      NodeCommaInserter operator+=( const NodePtr& node );

      /*! Proxy classs for deleting comma separated nodes */
      class NodeCommaDeleter
      {
      public:
	/*! Deletes node from parent's childs */
	NodeCommaDeleter( Node& parent, NodePtr& node );
	/*! Deletes one more node */
	NodeCommaDeleter operator,( NodePtr& node );

      private:
	Node& _parent;

      };

      /*! Deletes some comma separated nodes */
      NodeCommaDeleter operator-=( NodePtr& node );

      /*! Count of childs */
      size_t size() const;
      /*! Gets child by index */
      NodePtr& operator[]( size_t index );
	
      /*! An index of specified child */
      size_t index( const NodePtr& node ) const;
      /*! An index of specified child */
      size_t index( const Node* node ) const;
		    
    private:
      friend class Node;
      NodeChildsProxy( Node& parent, std::vector< NodePtr >& childs );

      Node& _parent;
      std::vector< NodePtr >& _childs;

    };
  };
}
#endif
