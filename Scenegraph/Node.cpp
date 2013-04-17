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

#include <boost/foreach.hpp>
#include "Scenegraph.hpp"
#include "Node.hpp"

using std::string;
using std::vector;
using namespace rtrt::scenegraph;

Node::Node(): _parent( NULL ),
	      _scenegraph( NULL ),
	      _name( "unnamed" )
{
}

Node::~Node()
{
  removeFromScenegraph();
  clearChilds();
}

void Node::removeFromScenegraph()
{
  BOOST_FOREACH( NodePtr& child, _childs )
    child->removeFromScenegraph();

  if( _scenegraph != NULL && _parent == NULL )
  {
    delete _scenegraph;
  }

  _scenegraph = NULL;
}

Node::Node( const Node& node ):
  _parent( NULL ),
  _scenegraph( NULL ),
  _name( node._name )
{
  copyChilds( node );
}

void Node::clearChilds()
{
  BOOST_FOREACH( NodePtr& child, _childs )
    child->_parent = NULL;

  _childs.clear();
}

void Node::copyChilds( const Node& node )
{
  BOOST_FOREACH( const NodePtr& child, node._childs )
  {
    childs() += Node::create( child );
  }
}

NodePtr Node::create( const NodePtr& node, Node* parent )
{
  NodePtr ret( new Node( *node ));

  if( parent != NULL )
    parent->childs() += ret;

  return ret;
}

NodePtr Node::create( const KernelPtr& kernel, NodePtr parent )
{
  return Node::create( kernel, parent.get() );
}

NodePtr Node::create( const KernelPtr& kernel, Node* parent )
{
  NodePtr ret( Node::create( NodePtr( new Node ), parent ));
  ret->setKernel( kernel );

  return ret;
}

NodePtr Node::createWithParent( Node* parent )
{
  return Node::create( NodePtr( new Node()), parent );
}

void Node::operator=( const Node& node )
{
  clearChilds();
  copyChilds( node );
}

const string& Node::name() const
{
  return _name;
}

void Node::setName( const std::string& name )
{
  _name = name;
}

Node* Node::parent() const
{
  return _parent;
}

Node::NodeChildsProxy Node::childs()
{
  return NodeChildsProxy( *this, _childs );
}

Scenegraph* Node::scenegraph() const
{
  return _scenegraph;
}

void Node::update( double time )
{
  if( _scenegraph == NULL )
    makeScenegraph();

  _scenegraph->update( time );
}

void Node::makeScenegraph()
{
  if( _parent != NULL )
  {
    _parent->makeScenegraph();
    return;
  }

  addToScenegraph( new Scenegraph( this ));
}

void Node::addChild( NodePtr node )
{
  if( node->parent() != NULL )
    node->parent()->removeChild( node );

  _childs.push_back( node );
  node->_parent = this;

  if( _scenegraph != NULL )
    node->addToScenegraph( _scenegraph );
  else
    node->removeFromScenegraph();
}

// временный хак
void Node::addChildSimple( NodePtr node )
{
  if( node->parent() != NULL )
    node->parent()->removeChild( node );

  _childs.push_back( node );
  node->_parent = this;
}

void Node::removeChild( NodePtr child )
{
  child->removeFromScenegraph();

  _childs.erase( std::remove( _childs.begin(), _childs.end(), child ), _childs.end() );
  child->_parent = NULL;
}

void Node::addToScenegraph( Scenegraph* scenegraph )
{
  if( _scenegraph != NULL )
    removeFromScenegraph();

  _scenegraph = scenegraph;
  if( _kernel != NULL )
    _scenegraph->addNode( this );

  BOOST_FOREACH( NodePtr& child, _childs )
    child->addToScenegraph( scenegraph );
}

const KernelPtr& Node::kernel() const
{
  return _kernel;
}

void Node::setKernel( const KernelPtr& kernel )
{
  if( _scenegraph != NULL )
  {
    if( _kernel != NULL )
      _scenegraph->removeNode( this );
  }

  _kernel = kernel;
  _kernel->assignNode( this );

  if( _scenegraph != NULL )
    _scenegraph->addNode( this );
}

void Node::removeKernel()
{
  if( _kernel == NULL )
    return;

  if( _scenegraph != NULL )
    _scenegraph->removeNode( this );

  _kernel->assignNode( NULL );
  _kernel.reset();
}

void Node::removeChildrenKernels()
{
  BOOST_FOREACH( NodePtr& child, _childs )
  {
    child->removeKernel();
    child->removeChildrenKernels();
  }
}

Node::NodeChildsProxy::NodeChildsProxy( Node& parent, vector< NodePtr >& childs ): 
  _parent( parent ),
  _childs( childs )
{
}

Node::NodeChildsProxy::NodeCommaInserter Node::NodeChildsProxy::operator+=( const NodePtr& node )
{
  return NodeCommaInserter( _parent, node );
}

Node::NodeChildsProxy::NodeCommaInserter::NodeCommaInserter( Node& parent, const NodePtr& node ):
  _parent( parent )
{
  _parent.addChild( node );
}

Node::NodeChildsProxy::NodeCommaInserter Node::NodeChildsProxy::NodeCommaInserter::operator,( const NodePtr& node )
{
  return NodeCommaInserter( _parent, node );
}

Node::NodeChildsProxy::NodeCommaDeleter Node::NodeChildsProxy::operator-=( NodePtr& node )
{
  return NodeCommaDeleter( _parent, node );
}

Node::NodeChildsProxy::NodeCommaDeleter::NodeCommaDeleter( Node& parent, NodePtr& node ):
  _parent( parent )
{
  _parent.removeChild( node );
}

Node::NodeChildsProxy::NodeCommaDeleter Node::NodeChildsProxy::NodeCommaDeleter::operator,( NodePtr& node )
{
  return NodeCommaDeleter( _parent, node );
}

size_t Node::NodeChildsProxy::size() const
{
  return _childs.size();
}

NodePtr& Node::NodeChildsProxy::operator[]( size_t index )
{
  assert( index < _childs.size() );

  return _childs[ index ];
}

size_t Node::NodeChildsProxy::index( const NodePtr& node ) const
{
  return std::distance( _childs.begin(),
                        std::find( _childs.begin(), _childs.end(), node ) );
}

class node_find
{
   const Node* _node;
public:
   node_find( const Node* node ) : _node( node ) {}
   bool operator() ( const NodePtr& node_ptr ) { return node_ptr.get() == _node; }
};

size_t Node::NodeChildsProxy::index( const Node* node ) const
{

  node_find find_pred( node );
  return std::distance( _childs.begin(),
                        std::find_if( _childs.begin(), _childs.end(), find_pred ) );
}

AttributeList& Node::attributes() { return _attributes; }
