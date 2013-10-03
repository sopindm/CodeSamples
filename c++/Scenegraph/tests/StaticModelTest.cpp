#include <gtest/gtest.h>
#include <boost/assign/std/vector.hpp>
#include <Scenegraph/StaticModel.hpp>

using namespace boost::assign;
using namespace Eigen;
using namespace rtrt;
using namespace rtrt::scenegraph;
using namespace rtrt::scenegraph::modelStash;

class StaticModelTest : public testing::Test
{
public:
  EIGEN_MAKE_ALIGNED_OPERATOR_NEW
  void SetUp();
  
  StaticModel sampleModel;

  static VertexArray sampleVertices();
  static NormalArray sampleNormals();
  static TextureArray sampleTextures();
  static IndexArray sampleIndices();
};

TEST_F( StaticModelTest, EmptyNodeCreation )
{
  StaticModel model;

  EXPECT_EQ( model.vertices().size(), 0 );
  EXPECT_EQ( model.normals().size(), 0 );
  EXPECT_EQ( model.textures().size(), 0 );
  EXPECT_EQ( model.indices().size(), 0 );
  ASSERT_EQ( model.stashes().size(), 1 );
  EXPECT_EQ( model.stashes()[ 0 ], "ModelStash" );
}

VertexArray StaticModelTest::sampleVertices()
{
  VertexArray vertices;
  vertices += Vector3f( 0.0, 0.0, 0.0 ),
    Vector3f( 1.0, 0.0, 0.0 ),
    Vector3f( 0.0, 1.0, 0.0 ),
    Vector3f( 1.0, 1.0, 0.0 );

  return vertices;
}

NormalArray StaticModelTest::sampleNormals()
{
  NormalArray normals;
  normals += Vector3f( 0.8, 0.6, 0.0 ),
    Vector3f( 0.8, 0.6, 0.0 ),
    Vector3f( 0.8, 0.6, 0.0 ),
    Vector3f( 0.8, 0.6, 0.0 );

  return normals;
}

TextureArray StaticModelTest::sampleTextures()
{
  Array2f textures;
  textures += Vector2f( 0.0, 0.5 ),
    Vector2f( 1.0, 0.0 ),
    Vector2f( 0.5, 0.7 ),
    Vector2f( 0.8, 0.2 );

  return textures;
}

IndexArray StaticModelTest::sampleIndices()
{
  IndexArray indices;
  indices += Vector3i( 0, 1, 2 ), 
             Vector3i( 0, 1, 3 );

  return indices;
}

void StaticModelTest::SetUp()
{
  sampleModel.setVertices( sampleVertices() );
  sampleModel.setNormals( sampleNormals() );
  sampleModel.setTextures( sampleTextures() );
  sampleModel.setIndices( sampleIndices() );
}

TEST_F( StaticModelTest, ConstructionTest )
{
  StaticModel model( sampleVertices(), sampleNormals(), sampleTextures(), sampleIndices() );

  EXPECT_TRUE( model.vertices() == sampleVertices() );
  EXPECT_TRUE( model.normals() == sampleNormals() );
  EXPECT_TRUE( model.textures() == sampleTextures() );
  EXPECT_TRUE( model.indices() == sampleIndices() );
}

TEST_F( StaticModelTest, sampleModelInitialization )
{
  ASSERT_EQ( sampleModel.vertices().size(), 4 );
  EXPECT_TRUE( sampleModel.vertices() == sampleVertices() );

  ASSERT_EQ( sampleModel.normals().size(), 4 );
  EXPECT_TRUE( sampleModel.normals() == sampleNormals() );

  ASSERT_EQ( sampleModel.textures().size(), 4 );
  EXPECT_TRUE( sampleModel.textures() == sampleTextures() );

  ASSERT_EQ( sampleModel.indices().size(), 2 );
  EXPECT_TRUE( sampleModel.indices() == sampleIndices() );
}


