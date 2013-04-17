#include <gtest/gtest.h>
#include <Scenegraph/Scenegraph.hpp>
#include <Scenegraph/TransformKernel.hpp>
#include <Scenegraph/Lights/PointLight.hpp>

using namespace Eigen;
using namespace rtrt;
using namespace rtrt::scenegraph;

class LightStashTest : public testing::Test
{
public:
  void SetUp();

  PointLightStash stash;

  void checkStash();
  void checkStash( const PointLight& light );
  void checkStash( const PointLight& light1, const PointLight& light2 );
  void checkStash( const PointLight& light1, const PointLight& light2, const PointLight& light3 );

  NodePtr node1, node2, node3;
  PointLightPtr light1, light2, light3;
};

void LightStashTest::checkStash()
{
  EXPECT_EQ( stash.array< 0 >().size(), 0 );
  EXPECT_EQ( stash.array< 1 >().size(), 0 );
}

void LightStashTest::checkStash( const PointLight& light )
{
  EXPECT_EQ( stash.array< 0 >().size(), 1 );
  EXPECT_TRUE( stash.array< 0 >()[ 0 ] == light.position() );

  EXPECT_EQ( stash.array< 1 >().size(), 1 );
  EXPECT_TRUE( stash.array< 1 >()[ 0 ] == light.intensity() );
}

void LightStashTest::checkStash( const PointLight& light1, const PointLight& light2 )
{
  EXPECT_EQ( stash.array< 0 >().size(), 2 );
  EXPECT_TRUE( stash.array< 0 >()[ 0 ] == light1.position() );
  EXPECT_TRUE( stash.array< 0 >()[ 1 ] == light2.position() );

  EXPECT_EQ( stash.array< 1 >().size(), 2 );
  EXPECT_TRUE( stash.array< 1 >()[ 0 ] == light1.intensity() );
  EXPECT_TRUE( stash.array< 1 >()[ 1 ] == light2.intensity() );
}

void LightStashTest::checkStash( const PointLight& light1, const PointLight& light2, const PointLight& light3 )
{
  EXPECT_EQ( stash.array< 0 >().size(), 3 );
  EXPECT_TRUE( stash.array< 0 >()[ 0 ] == light1.position() );
  EXPECT_TRUE( stash.array< 0 >()[ 1 ] == light2.position() );
  EXPECT_TRUE( stash.array< 0 >()[ 2 ] == light3.position() );

  EXPECT_EQ( stash.array< 1 >().size(), 3 );
  EXPECT_TRUE( stash.array< 1 >()[ 0 ] == light1.intensity() );
  EXPECT_TRUE( stash.array< 1 >()[ 1 ] == light2.intensity() );
  EXPECT_TRUE( stash.array< 1 >()[ 2 ] == light3.intensity() );
}

void LightStashTest::SetUp()
{
  light1 = PointLight::create( Vector3f( 1., 2., 3. ), Color3f( 2.1, 3.1, 4.2 ));
  light2 = PointLight::create( Vector3f( 4., 5., 6. ), Color3f( 1.1, 0.1, 23.2 ));
  light3 = PointLight::create( Vector3f( 7., 8., 9. ), Color3f( 8.1, 7., 4.243 ));

  node1 = Node::create( light1 );
  node2 = Node::create( light2 );
  node3 = Node::create( light3 );
}

TEST_F( LightStashTest, EmptyStash )
{
  checkStash();
}

TEST_F( LightStashTest, AddingLight )
{
  stash.addObject( node1.get() );
  light1->allocate();
  stash.writeObject( node1.get() );

  checkStash( *light1 );

  light1->free();
  stash.removeObject( node1.get() );

  checkStash();
}

TEST_F( LightStashTest, AddingSeveralObjects )
{
  stash.addObject( node1.get() );
  stash.addObject( node2.get() );
  stash.addObject( node3.get() );

  light1->allocate();
  light2->allocate();
  light3->allocate();

  stash.writeObject( node1.get() );
  stash.writeObject( node2.get() );
  stash.writeObject( node3.get() );

  checkStash( *light1, *light2, *light3 );

  light2->free();
  checkStash( *light1, *light3 );

  stash.removeObject( node2.get() );
  checkStash( *light1, *light3 );

  light1->free();
  stash.removeObject( node1.get() );
  
  checkStash( *light3 );

  light3->free();
  stash.removeObject( node3.get() );
  checkStash();
}

TEST_F( LightStashTest, RemovingObjects )
{
  stash.addObject( node1.get() );
  stash.addObject( node2.get() );
  stash.addObject( node3.get() );

  light1->allocate();
  light2->allocate();
  light3->allocate();

  light1->free();
  light2->free();

  stash.writeObject( node3.get() );
  checkStash( *light3 );
}

TEST_F( LightStashTest, TransformTest )
{
  TransformKernelPtr kernel( TransformKernel::createScaling( 2., 3., 4. ));
  NodePtr node( Node::create( kernel ));
  node->childs() += node1;
  
  node->update( 0.0 );

  PointLightStashPtr stash = SCENEGRAPH_STASH( PointLightStash, node );
  ASSERT_TRUE( stash != NULL );

  ASSERT_EQ( stash->array< 0 >().size(), 1 );
  EXPECT_TRUE( stash->array< 0 >()[ 0 ] == Vector3f( 2., 6., 12. ));

  ASSERT_EQ( stash->array< 1 >().size(), 1 );
  EXPECT_TRUE( stash->array< 1 >()[ 0 ] == light1->intensity() );

  kernel->scale( 2., 2., 2. );
  node->update( 1.0 );

  EXPECT_FLOAT_EQ( stash->array< 0 >()[ 0 ].x(), 4 );
  EXPECT_FLOAT_EQ( stash->array< 0 >()[ 0 ].y(), 12 );
  EXPECT_FLOAT_EQ( stash->array< 0 >()[ 0 ].z(), 24 );
}

TEST_F( LightStashTest, stashUpdateTest )
{
  Time time( 213 );
  EXPECT_TRUE( stash.isOld( time ));

  stash.addObject( node1.get() );
  light1->allocate();

  stash.resolveNewest( time );
  time++;
  EXPECT_TRUE( stash.isOld( time ));

  light1->resolveNewest( time );
  stash.writeObject( node1.get() );
  EXPECT_TRUE( stash.isNew( time ));
  
  stash.resolveNewest( time );
  time++;
  EXPECT_TRUE( stash.isOld( time ));

  stash.writeObject( node1.get(), AttributeList(), time );
  EXPECT_TRUE( stash.isOld( time ));
}
