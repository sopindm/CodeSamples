#include <gtest/gtest.h>
#include <boost/foreach.hpp>
#include <boost/assign/std/vector.hpp>
#include "StaticModels.hpp"

using namespace boost::assign;
using namespace Eigen;

void StaticModels::makeModel1()
{
  VertexArray vertices;
  vertices += Vector3f( 1.0, 2.0, 3.0 ),
    Vector3f( 1.5, 0.5, -3.7 );
  
  NormalArray normals;
  normals += Vector3f( 0.2, 0.3, 0.5 ).normalized(),
    Vector3f( 0.3, 0.7, 0.2 ).normalized();

  TextureArray textures;
  textures += Vector2f( 1.2, 0.1 ),
    Vector2f( 0.3, 0.8 );

  IndexArray indices;
  indices += Vector3i( 0, 1, 0 ),
    Vector3i( 1, 1, 0 );

  model1.reset( new StaticModel );

  model1->setVertices( vertices );
  model1->setNormals( normals );
  model1->setTextures( textures );
  model1->setIndices( indices );

  node1->setKernel( model1 );
}

void StaticModels::makeModel2()
{
  VertexArray vertices;
  vertices += Vector3f( 2.0, -3.0, 4.3 ),
    Vector3f( -0.7, 1.2, 2.2 ),
    Vector3f( 12.6, -8.2, 17.1 ),
    Vector3f( -100.23, 12.3, 14.8 );

  NormalArray normals;
  normals += Vector3f( 0.7, 0.1, 0.4 ).normalized(),
    Vector3f( 1.2, 0.1, 2.8 ).normalized(),
    Vector3f( 11.1, 1.1, 2.2 ).normalized(),
    Vector3f( -3.4, 0.35, 12.16 ).normalized();

  TextureArray textures;
  textures+= Vector2f( 0.8, 0.2),
    Vector2f( 0.4, 0.3 ),
    Vector2f( 0.8, 0.95),
    Vector2f( 0.1, 0.76 );

  IndexArray indices;
  indices += Vector3i( 0, 1, 2 ),
    Vector3i( 0, 1, 3 ),
    Vector3i( 0, 2, 3 ),
    Vector3i( 1, 2, 3 );

  model2.reset( new StaticModel );

  model2->setVertices( vertices );
  model2->setNormals( normals );
  model2->setTextures( textures );
  model2->setIndices( indices );

  node2->setKernel( model2 );
}

void StaticModels::makeModel3()
{
  VertexArray vertices;
  vertices += Vector3f( 3.0, 1.0, 2.0),
    Vector3f( 2.3, 1.4, 2.2 );

  NormalArray normals;
  normals += Vector3f( 2.8, -3.6, 2.5 ).normalized(),
    Vector3f( 1.0, 2.2, 1.3 ).normalized();
  
  TextureArray textures;
  textures += Vector2f( 0.3, 0.7 ),
    Vector2f( 0.5, 0.2 );

  IndexArray indices;
  indices += Vector3i( 0, 1, 0 ),
    Vector3i( 1, 0, 1 );

  model3.reset( new StaticModel );

  model3->setVertices( vertices );
  model3->setNormals( normals );
  model3->setTextures( textures );
  model3->setIndices( indices );

  node3->setKernel( model3 );
}

void StaticModels::checkStash()
{
  ASSERT_EQ( stash->vertices().size(), 0 );
  ASSERT_EQ( stash->normals().size(), 0 );
  ASSERT_EQ( stash->textures().size(), 0 );
  ASSERT_EQ( stash->indices().size(), 0 );
  ASSERT_EQ( stash->objectIndices().size(), 0 );
}

void StaticModels::checkStash( const StaticModel& model, size_t index )
{
  std::vector< uint32_t > objectIndices( model.indices().size(), index );

  EXPECT_TRUE( stash->vertices() == model.vertices() );
  checkArray3f( stash->normals(), model.normals() );
  EXPECT_TRUE( stash->textures() == model.textures() );
  EXPECT_TRUE( stash->indices() == model.indices() );
  EXPECT_TRUE( stash->objectIndices() == objectIndices );
}

void StaticModels::checkStash( const StaticModel& model1, const StaticModel& model2, size_t index1, size_t index2 )
{
  typedef std::vector< uint32_t > Vector;
  Vector objectIndices =  Vector( model1.indices().size(), index1 ) + Vector( model2.indices().size(), index2 );

  IndexArray indices( model2.indices() );
  size_t size( model1.indices().size() );
  BOOST_FOREACH( Vector3i& index, indices )
    index += Vector3i( size, size, size );

  EXPECT_TRUE( stash->vertices() == (model1.vertices() + model2.vertices() ));
  checkArray3f( stash->normals(), model1.normals() + model2.normals() );
  EXPECT_TRUE( stash->textures() == (model1.textures() + model2.textures() ));
  EXPECT_TRUE( stash->indices() == (model1.indices() + indices ));
  EXPECT_TRUE( stash->objectIndices() == objectIndices );
}

void StaticModels::checkStash( const StaticModel& model1, 
				 const StaticModel& model2, 
				 const StaticModel& model3 )
{
  typedef std::vector< uint32_t > Vector;
  Vector objectIndices =  Vector( model1.indices().size(), 0 )
    + Vector( model2.indices().size(), 1 )
    + Vector( model3.indices().size(), 2 );

  IndexArray indices2( model2.indices() );
  size_t size( model1.indices().size() );
  BOOST_FOREACH( Vector3i& index, indices2 )
    index += Vector3i( size, size, size );

  IndexArray indices3( model3.indices() );
  size += model2.indices().size();
  BOOST_FOREACH( Vector3i& index, indices3 )
    index += Vector3i( size, size, size );

  EXPECT_TRUE( stash->vertices() == (model1.vertices() + model2.vertices() + model3.vertices() ));
  checkArray3f( stash->normals(), model1.normals() + model2.normals() + model3.normals() );
  EXPECT_TRUE( stash->textures() == (model1.textures() + model2.textures() + model3.textures() ));
  EXPECT_TRUE( stash->indices() == (model1.indices() + indices2 + indices3 ));
  EXPECT_TRUE( stash->objectIndices() == objectIndices );
}

void StaticModels::setup()
{
  node1.reset( new Node() );
  node2.reset( new Node() );
  node3.reset( new Node() );

  makeModel1();
  makeModel2();
  makeModel3();

  model1->assignNode( node1.get() );
  model2->assignNode( node2.get() );  
  model3->assignNode( node3.get() );

  stash.reset( new ModelStash );
}
