#include <gtest/gtest.h>
#include <Scenegraph/Scenegraph.hpp>
#include <Scenegraph/TransformKernel.hpp>
#include <Scenegraph/StaticModel.hpp>

using namespace Eigen;
using namespace rtrt::scenegraph;

class TransformKernelTest : public testing::Test
{
public:
  void SetUp();
  void checkIdentity( const Transform3f& transform );
  
  TransformKernel kernel;
  Node node;
};

void TransformKernelTest::SetUp()
{
  kernel.assignNode( &node );
}

void TransformKernelTest::checkIdentity( const Transform3f& transform )
{
  EXPECT_TRUE( transform.isApprox( static_cast< Transform3f >( Eigen::Transform3f::Identity() )));
}

TEST_F( TransformKernelTest, DefaultInitialization )
{
  checkIdentity( *kernel.transform() );
}

TEST_F( TransformKernelTest, UpdateTest )
{
  Time time( Time::lowest );

  kernel.update( 0.0 );

  TransformAttributePtr attr( SCENEGRAPH_ATTRIBUTE( Transform, node.attributes() ) );
  EXPECT_TRUE( attr->isOld( time ));

  kernel.rotate( 1.57, 0.0, 0.0 );
  kernel.update( 0.0 );

  attr = SCENEGRAPH_ATTRIBUTE( Transform, node.attributes() );
  EXPECT_TRUE( attr->isNew( time ));
}


TEST_F( TransformKernelTest, TranslationTest )
{
  kernel.translate( 1.0, 2.0, 3.0 );
  kernel.update( 0.0 );

  TransformAttributePtr attr( SCENEGRAPH_ATTRIBUTE( Transform, node.attributes() ));
  EXPECT_TRUE( attr->isNew( Time( Time::lowest )) );

  Vector3f value( attr->value() * Vector3f( 0., 0., 0.  ));
  EXPECT_TRUE( value == Vector3f( 1., 2., 3. ) );
}

TEST_F( TransformKernelTest, ScalingTest )
{
  kernel.scale( 2.0, 0.5, 3.0 );
  kernel.update( 0.0 );

  TransformAttributePtr attr( SCENEGRAPH_ATTRIBUTE( Transform, node.attributes() ));
  EXPECT_TRUE( attr->isNew( Time( Time::lowest )));

  Vector3f zeroValue( attr->value() * Vector3f( 0., 0., 0. ) );
  EXPECT_TRUE( zeroValue == Vector3f( 0., 0., 0. ) );

  Vector3f value( 0.5, 2.0, 1.0 );
  value = attr->value() * value;

  EXPECT_TRUE( value == Vector3f( 1., 1., 3. ) );
}

TEST_F( TransformKernelTest, MergingTest )
{
  NodePtr node( Node::create());

  TransformKernelPtr kernel( TransformKernel::createTranslation( 1.0, 2.0, 3.0 ));
  node->setKernel( kernel );

  NodePtr child( Node::create( TransformKernel::createScaling( 1.0, 2.0, 3.0 )));

  node->childs() += child;

  modelStash::VertexArray vertices;
  vertices.push_back( Vector3f( 0., 0., 0. ));
  vertices.push_back( Vector3f( 1., 1., 1. ));

  StaticModelPtr model( new StaticModel );
  model->setVertices( vertices );

  child->childs() += Node::create( model );

  node->update( 0.0 );

  ModelStashPtr stash = SCENEGRAPH_STASH( ModelStash, node );

  ASSERT_EQ( stash->vertices().size(), 2 );

  EXPECT_TRUE( stash->vertices()[ 0 ] == Vector3f( 1., 2., 3. ));
  EXPECT_TRUE( stash->vertices()[ 1 ] == Vector3f( 2., 4., 6. ));
}

TEST_F( TransformKernelTest, DeadTransform )
{
  NodePtr node( Node::create( TransformKernel::createTranslation( 1., 2., 3. )));

  modelStash::VertexArray vertices;
  vertices.push_back( Vector3f( 0., 0., 0. ));
  vertices.push_back( Vector3f( 1., 1., 1. ));

  StaticModelPtr model( new StaticModel );
  model->setVertices( vertices );

  node->childs() += Node::create( model );

  node->update( 0.0 );

  node->removeKernel();
  node->update( 1.0 );

  ModelStashPtr stash = SCENEGRAPH_STASH( ModelStash, node );

  ASSERT_EQ( stash->vertices().size(), 2 );

  EXPECT_TRUE( stash->vertices()[ 0 ] == Vector3f( 0., 0., 0. ));
  EXPECT_TRUE( stash->vertices()[ 1 ] == Vector3f( 1., 1., 1. ));
}
