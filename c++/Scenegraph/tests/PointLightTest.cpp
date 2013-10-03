#include <gtest/gtest.h>
#include <Scenegraph/Lights/PointLight.hpp>

using namespace Eigen;
using namespace rtrt;
using namespace rtrt::scenegraph;

class PointLightTest : public testing::Test
{
public:
  void SetUp();

  void checkVector( const Vector3f& vector, const Vector3f& expected );
  void checkVector( const Vector4f& vector, const Vector4f& expected );
};

void PointLightTest::checkVector( const Vector3f& vector, const Vector3f& expected )
{
  EXPECT_FLOAT_EQ( vector.x(), expected.x() );
  EXPECT_FLOAT_EQ( vector.y(), expected.y() );
  EXPECT_FLOAT_EQ( vector.z(), expected.z() );
}

void PointLightTest::checkVector( const Vector4f& vector, const Vector4f& expected )
{
  EXPECT_FLOAT_EQ( vector.x(), expected.x() );
  EXPECT_FLOAT_EQ( vector.y(), expected.y() );
  EXPECT_FLOAT_EQ( vector.z(), expected.z() );
  EXPECT_FLOAT_EQ( vector.w(), expected.w() );
}

void PointLightTest::SetUp()
{
}

TEST_F( PointLightTest, Accessing )
{
  PointLight light( Color3f::Zero(), Color3f::Zero() );

  light.setPosition( Vector3f( 1., 2., 3. ));
  checkVector( light.position(), Vector3f( 1., 2., 3. ));
  light.setIntensity( Color3f( 0.5, 0.6, 0.7 ));
  checkVector( light.intensity(), Color3f( 0.5, 0.6, 0.7 ));
}

TEST_F( PointLightTest, UpdateTest )
{
  PointLight light( Color3f::Zero(), Color3f::Zero() );
  Time time( 213 );

  EXPECT_TRUE( light.isOld( time ));

  light.setPosition( Vector3f( 0., 1., 2. ));
  EXPECT_TRUE( light.isNew( time ));

  light.resolveNewest( time );
  time++;

  EXPECT_TRUE( light.isOld( time ));

  light.setIntensity( Color3f( 1., 2., 3. ));
  EXPECT_TRUE( light.isNew( time ));
}
