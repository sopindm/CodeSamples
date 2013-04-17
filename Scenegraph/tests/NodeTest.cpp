#include <gtest/gtest.h>
#include <Scenegraph/Node.hpp>
#include <Scenegraph/Scenegraph.hpp>
#include <Scenegraph/StaticModel.hpp>
#include "StaticModels.hpp"

using namespace Eigen;
using namespace rtrt::scenegraph;

class NodeTest : public testing::Test, public StaticModels
{
public:
  static void checkChilds( Node& node, size_t expected_size, bool emptyChilds = true );

  void SetUp();
};

void NodeTest::SetUp()
{
  setup();
}

void NodeTest::checkChilds( Node& node, size_t expected_size, bool emptyChilds )
{
  ASSERT_EQ( node.childs().size(), expected_size );

  for( size_t i=0; i<expected_size; i++ )
  {
    Node& child( *node.childs()[ i ] );

    if( emptyChilds )
      EXPECT_EQ( child.childs().size(), 0 );

    EXPECT_EQ( child.parent(), &node );
    EXPECT_EQ( child.scenegraph(), node.scenegraph() );
  }
}

TEST_F( NodeTest, EmptyNode )
{
  Node node;
  
  EXPECT_EQ( node.childs().size(), 0 );
  EXPECT_TRUE( node.parent() == NULL );
}

TEST_F( NodeTest, AddingChilds )
{
  Node node;
  node.childs() += Node::create(), Node::create(), Node::create();
  checkChilds( node, 3 );
}

TEST_F( NodeTest, RemovingChilds )
{
  Node node;

  NodePtr child1( Node::create() );
  NodePtr child2( Node::create() );
  NodePtr child3( Node::create() );

  node.childs() += child1, child2, child3;
  checkChilds( node, 3 );

  node.childs() -= child1;
  checkChilds( node, 2 );

  EXPECT_TRUE( child1->parent() == NULL );

  node.childs() -= child2, child3;
  EXPECT_EQ( node.childs().size(), 0 );

  EXPECT_TRUE( child2->parent() == NULL );
  EXPECT_TRUE( child3->parent() == NULL );
}

TEST_F( NodeTest, DestructionTest )
{
  NodePtr child1( Node::create() );
  NodePtr child2( Node::create() );
    
  {
    Node node;
    node.childs() += child1, child2;

    checkChilds( node, 2 );
  }

  EXPECT_TRUE( child1->parent() == NULL );
  EXPECT_TRUE( child2->parent() == NULL );
}

TEST_F( NodeTest, CopyTest )
{
  Node node;
  node.childs() += Node::create(), Node::create();

  Node node2( node );
  checkChilds( node2, 2 );

  EXPECT_NE( node2.childs()[ 0 ], node.childs()[ 0 ] );
  EXPECT_NE( node2.childs()[ 1 ], node.childs()[ 1 ] );
}

TEST_F( NodeTest, AssignTest )
{
  NodePtr child1( Node::create() );
  NodePtr child2( Node::create() );
  
  Node node;
  node.childs() += child1, child2;

  Node node2;
  node2.childs() += Node::create(), Node::create(), Node::create();
  
  node = node2;
  checkChilds( node, 3 );

  EXPECT_TRUE( child1->parent() == NULL );
  EXPECT_TRUE( child2->parent() == NULL );

  EXPECT_NE( node.childs()[ 0 ], node2.childs()[ 0 ] );
  EXPECT_NE( node.childs()[ 1 ], node2.childs()[ 1 ] );
  EXPECT_NE( node.childs()[ 2 ], node2.childs()[ 2 ] );
}

TEST_F( NodeTest, DefaultScenegraph )
{
  Node node;
  EXPECT_TRUE( node.scenegraph() == NULL );

  Node node2( node );
  EXPECT_TRUE( node2.scenegraph() == NULL );
}

TEST_F( NodeTest, UpdateCreatesScenegraph )
{
  Node node;

  node.update( 0.0 );
  EXPECT_FALSE( node.scenegraph() == NULL );

  Scenegraph* scenegraph( node.scenegraph() );
  
  node.update( 1.0 );
  EXPECT_EQ( node.scenegraph(), scenegraph );
}

TEST_F( NodeTest, AddingScenegraphToChilds )
{
  NodePtr child( Node::create() );
  child->childs() += Node::create(), Node::create();

  Node node;
  node.childs() += Node::create(), child, Node::create();

  node.update( 1.0 );
  checkChilds( node, 3, false );
  checkChilds( *child, 2 );
}

TEST_F( NodeTest, AddingScenegraphToParent )
{
  Node parent;
  NodePtr node( Node::create() );

  parent.childs() += node;

  node->update( 1.0 );

  checkChilds( parent, 1 );
}

TEST_F( NodeTest, AddingScenegraphToNewChilds )
{
  Node node;
  node.update( 0.0 );

  node.childs() += Node::create(), Node::create();

  checkChilds( node, 2 );
}

TEST_F( NodeTest, UpdateUpdatesScenegraph )
{
  Node node;
  node.update( 0.0 );

  Time time( node.scenegraph()->time() );

  node.update( -1.0 );
  
  EXPECT_TRUE( node.scenegraph()->time() > time );
}

TEST_F( NodeTest, SharingScenegraphWithParent )
{
  NodePtr node( Node::create() );
  node->update( 0.0 );
  
  Node root;
  root.childs() += node;

  checkChilds( root, 1 );
  EXPECT_TRUE( root.scenegraph() == NULL );
}

TEST_F( NodeTest, ScenegraphRoot )
{
  Node root;
  NodePtr child( Node::create() );
  NodePtr subchild( Node::create() );

  root.childs() += child;
  child->childs() += subchild;

  subchild->update( 0. );

  ASSERT_TRUE( root.scenegraph() != NULL );
  EXPECT_EQ( root.scenegraph()->root(), &root );
}

TEST_F( NodeTest, CreatingNodeWithParent )
{
  Node root;

  NodePtr child( Node::createWithParent( &root ));
  child->childs() += Node::create();

  NodePtr child2( Node::create( child, &root ));

  checkChilds( root, 2, false );
  checkChilds( *child, 1 );
  checkChilds( *child2, 1 );
}

TEST_F( NodeTest, AddingChildTwice )
{
  Node parent;
  NodePtr child( Node::create() );

  parent.childs() += child, child;

  checkChilds( parent, 1 );
}

TEST_F( NodeTest, ChangingParent )
{
  NodePtr child( Node::create() );

  Node parent1;
  parent1.childs() += child;

  Node parent2;
  parent2.childs() += child;
  
  checkChilds( parent1, 0 );
  checkChilds( parent2, 1 );
}

TEST_F( NodeTest, CreatingWithKernel )
{
  StaticModelPtr model( new StaticModel );
  NodePtr node( Node::create( model ));

  EXPECT_EQ( node->kernel(), model );
  EXPECT_TRUE( node->scenegraph() == NULL );
}

TEST_F( NodeTest, SimpleScenegraph )
{
  modelStash::VertexArray vertices;
  vertices.push_back( Vector3f( 1.0, 2.0, 3.0 ));

  StaticModelPtr model( new StaticModel );
  model->setVertices( vertices );

  NodePtr node( Node::create( model ));
  node->update( 0.0 );

  ModelStashPtr stash = SCENEGRAPH_STASH( ModelStash, node );
  ASSERT_TRUE( stash != NULL );

  ASSERT_EQ( stash->vertices().size(), 1 );
  EXPECT_TRUE( stash->vertices()[ 0 ] == vertices[ 0 ] );

  EXPECT_EQ( stash->normals().size(), 0 );
  EXPECT_EQ( stash->textures().size(), 0 );
  EXPECT_EQ( stash->indices().size(), 0 );
  EXPECT_EQ( stash->objectIndices().size(), 0 );
}

TEST_F( NodeTest, SeveralNodesScenegraph )
{
  node1->childs() += node2;
  node2->childs() += node3;

  node3->update( 0.0 );

  stash = SCENEGRAPH_STASH( ModelStash, node2 );
  ASSERT_TRUE( stash != NULL );

  checkStash( *model1, *model2, *model3 );
}

TEST_F( NodeTest, RemovingKernel )
{
  node1->removeKernel();
  node1->update( 0.0 );

  stash = SCENEGRAPH_STASH( ModelStash, node1 );
  ASSERT_TRUE( stash == NULL );
}

TEST_F( NodeTest, RemovingKernelAfterUpdate )
{
  node1->childs() += node2;
  node1->update( 0.0 );

  node1->removeKernel();
  node1->update( 1. );

  stash = SCENEGRAPH_STASH( ModelStash, node1 );
  ASSERT_TRUE( stash != NULL );

  checkStash( *model2 );
}



