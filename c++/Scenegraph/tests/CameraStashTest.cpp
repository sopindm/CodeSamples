#include <gtest/gtest.h>
#include <Scenegraph/Scenegraph.hpp>
#include <Scenegraph/Lights/PointLight.hpp>
#include <Scenegraph/Camera.hpp>
#include <Scenegraph/CameraStash.hpp>
#include <Scenegraph/TransformKernel.hpp>

using namespace Eigen;
using namespace rtrt;
using namespace rtrt::scenegraph;
using namespace rtrt::scenegraph::cameraStash;

class CameraStashTest : public testing::Test
{
public:
  void SetUp();

  CameraStash stash;

  void checkStash();
  void checkStash( const BaseCameraPtr& camera );
  void checkStash( const BaseCameraPtr& camera1, const BaseCameraPtr& camera2 );
  void checkStash( const BaseCameraPtr& camera1, const BaseCameraPtr& camera2, const BaseCameraPtr& camera3 );

  NodePtr node1, node2, node3;
  CameraPtr camera1, camera2, camera3;
};

void CameraStashTest::checkStash()
{
  EXPECT_EQ( stash.cameras().size(), 0 );
}

void CameraStashTest::checkStash( const BaseCameraPtr& camera )
{
  EXPECT_EQ( stash.cameras().size(), 1 );
  EXPECT_EQ( stash.cameras()[ 0 ], camera );
}

void CameraStashTest::checkStash( const BaseCameraPtr& camera1, const BaseCameraPtr& camera2 )
{
  EXPECT_EQ( stash.cameras().size(), 2 );
  EXPECT_EQ( stash.cameras()[ 0 ], camera1 );
  EXPECT_EQ( stash.cameras()[ 1 ], camera2 );
}

void CameraStashTest::checkStash( const BaseCameraPtr& camera1, 
				  const BaseCameraPtr& camera2, 
				  const BaseCameraPtr& camera3 )
{
  EXPECT_EQ( stash.cameras().size(), 3 );
  EXPECT_EQ( stash.cameras()[ 0 ], camera1 );
  EXPECT_EQ( stash.cameras()[ 1 ], camera2 );
  EXPECT_EQ( stash.cameras()[ 2 ], camera3 );
}

void CameraStashTest::SetUp()
{
  camera1 = Camera::create();
  camera2 = Camera::create();
  camera3 = Camera::create();

  node1 = Node::create( camera1 );
  node2 = Node::create( camera2 );
  node3 = Node::create( camera3 );
}

TEST_F( CameraStashTest, EmptyStash )
{
  checkStash();
}

TEST_F( CameraStashTest, AddingCamera )
{
  stash.addObject( node1.get() );
  camera1->allocate();
  stash.writeObject( node1.get() );

  checkStash( camera1 );

  camera1->free();
  stash.removeObject( node1.get() );

  checkStash();
}

TEST_F( CameraStashTest, AddingSeveralObjects )
{
  stash.addObject( node1.get() );
  stash.addObject( node2.get() );
  stash.addObject( node3.get() );

  camera1->allocate();
  camera2->allocate();
  camera3->allocate();

  stash.writeObject( node1.get() );
  stash.writeObject( node2.get() );
  stash.writeObject( node3.get() );

  checkStash( camera1, camera2, camera3 );

  camera2->free();
  checkStash( camera1, camera3 );

  stash.removeObject( node2.get() );
  checkStash( camera1, camera3 );

  camera1->free();
  stash.removeObject( node1.get() );
  
  checkStash( camera3 );

  camera3->free();
  stash.removeObject( node3.get() );
  checkStash();
}

TEST_F( CameraStashTest, RemovingObjects )
{
  stash.addObject( node1.get() );
  stash.addObject( node2.get() );
  stash.addObject( node3.get() );

  camera1->allocate();
  camera2->allocate();
  camera3->allocate();

  camera1->free();
  camera2->free();

  stash.writeObject( node3.get() );
  checkStash( camera3 );
}

TEST_F( CameraStashTest, TransformTest )
{
  TransformKernelPtr kernel( TransformKernel::createScaling( 2., 3., 4. ));
  NodePtr node( Node::create( kernel ));
  node->childs() += node1;
  
  node->update( 0.0 );

  CameraStashPtr stash = SCENEGRAPH_STASH( CameraStash, node );
  ASSERT_TRUE( stash != NULL );

  EXPECT_TRUE( stash->cameras()[ 0 ]->baseTransform().isApprox( *kernel->transform() ));

  kernel->scale( 2., 2., 2. );
  node->update( 1.0 );

  EXPECT_TRUE( stash->cameras()[ 0 ]->baseTransform().isApprox( *kernel->transform() ));
}
