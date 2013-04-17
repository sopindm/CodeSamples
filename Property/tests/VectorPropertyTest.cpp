#include <gtest/gtest.h>
#include <boost/assign/std/vector.hpp>
#include <Property/AtomProperty.hpp>
#include <Property/VectorProperty.hpp>
#include <Property/ListProperty.hpp>
#include <Eigen/Core>
#include <Core/Types.hpp>
#include "SimpleUpdateHandler.hpp"

using namespace Eigen;
using std::vector;
using std::string;
using namespace rtrt;
using namespace rtrt::property;
using namespace boost::assign;
using namespace boost;

class VectorPropertyTest : public testing::Test
{
public:
  template< class T >
  void checkRawAccess();

  template< class T >
  void checkNoRawAccess();
};

template< class T >
void VectorPropertyTest::checkRawAccess()
{
  typename Type< T >::VectorType vector;

  VectorPropertyPtr property( SimpleVectorProperty< T >::create( "vector", "elt", vector ));
  property->pushBack();

  EXPECT_TRUE( property->rawData() != NULL );
  EXPECT_EQ( property->elementSize(), sizeof( T ) );
}

template< class T >
void VectorPropertyTest::checkNoRawAccess()
{
  typename Type< T >::VectorType vector;

  VectorPropertyPtr property( SimpleVectorProperty< T >::create( "vector", "elt", vector ));
  property->pushBack();

  EXPECT_TRUE( property->rawData() == NULL );
  EXPECT_EQ( property->elementSize(), 0 );
}

TEST_F( VectorPropertyTest, EmptyProperty )
{
  std::vector< int > ints;
  VectorPropertyPtr prop( SimpleVectorProperty< int >::create( "vec", "int", ints ) );

  EXPECT_EQ( prop->size(), 0 );
  EXPECT_EQ( prop->elementSize(), sizeof( int ));

  EXPECT_EQ( prop->link(), prop );
}

TEST_F( VectorPropertyTest, PushBackTest )
{
  std::vector< int > ints;
  VectorPropertyPtr prop( SimpleVectorProperty< int >::create( "vec", "int", ints ));

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
  prop->connect( handler );

  prop->pushBack();
  prop->pushBack();

  EXPECT_TRUE( handler->isUpdated() );

  ASSERT_EQ( ints.size(), 2 );
  EXPECT_EQ( ints[ 0 ], 0 );
  EXPECT_EQ( ints[ 1 ], 0 );

  EXPECT_EQ( prop->rawData(), &ints[ 0 ] );
}

TEST_F( VectorPropertyTest, AccessTest )
{
  std::vector< float > floats;
  VectorPropertyPtr prop( SimpleVectorProperty< float >::create( "vec", "float", floats ));

  prop->pushBack();
  prop->pushBack();

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
  prop->connect( handler );

  FloatPropertyPtr elt1( dynamic_pointer_cast< FloatProperty >( prop->at( 0 ) ));
  ASSERT_TRUE( elt1 != NULL );

  EXPECT_FALSE( handler->isUpdated() );

  EXPECT_EQ( elt1->get(), 0 );
  elt1->set( 10.1 );

  EXPECT_TRUE( handler->isUpdated() );

  FloatPropertyPtr elt2( dynamic_pointer_cast< FloatProperty >( prop->at( 1 ) ));
  ASSERT_TRUE( elt2 != NULL );
  
  EXPECT_EQ( elt2->get(), 0 );
  elt2->set( 20.2 );

  ASSERT_EQ( floats.size(), 2 );
  EXPECT_FLOAT_EQ( floats[ 0 ], 10.1 );
  EXPECT_FLOAT_EQ( floats[ 1 ], 20.2 );
}

TEST_F( VectorPropertyTest, InsertTest )
{
  std::vector< int > ints( 3, 100 );
  VectorPropertyPtr prop( SimpleVectorProperty< int >::create( "vec", "int", ints ));

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
  prop->connect( handler );

  prop->insert( 2 );

  EXPECT_TRUE( handler->isUpdated() );

  ASSERT_EQ( ints.size(), 4 );
  EXPECT_EQ( ints[ 2 ], 0 );
}

TEST_F( VectorPropertyTest, RemoveTest )
{
  std::vector< int > ints( 3, 100 );
  VectorPropertyPtr prop( SimpleVectorProperty< int >::create( "vec", "int", ints ));

  prop->insert( 1 );

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
  prop->connect( handler );

  EXPECT_FALSE( handler->isUpdated() );

  prop->remove( 0 );

  EXPECT_TRUE( handler->isUpdated() );

  ASSERT_EQ( ints.size(), 3 );
  EXPECT_EQ( ints[ 0 ], 0 );
}

TEST_F( VectorPropertyTest, ResizeTest )
{
  std::vector< float > floats( 2, 11.1 );

  VectorPropertyPtr prop( SimpleVectorProperty< float >::create( "vec", 
								 "float", 
								 floats, 
								 VectorConstructor< float >( 22.2 ) ));

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
  prop->connect( handler );

  prop->resize( 1 );

  EXPECT_TRUE( handler->isUpdated() );

  EXPECT_EQ( floats.size(), 1 );

  SimpleUpdateHandlerPtr handler2( new SimpleUpdateHandler() );
  prop->connect( handler2 );

  prop->resize( 3 );
  EXPECT_TRUE( handler2->isUpdated() );

  ASSERT_EQ( floats.size(), 3 );
  EXPECT_FLOAT_EQ( floats[ 0 ], 11.1 );
  EXPECT_FLOAT_EQ( floats[ 1 ], 22.2 );
  EXPECT_FLOAT_EQ( floats[ 2 ], 22.2 );
}

TEST_F( VectorPropertyTest, RawAccess )
{
  std::vector< int > ints( 3 );
  ints[ 0 ] = 1;
  ints[ 1 ] = 11;
  ints[ 2 ] = 111;

  VectorPropertyPtr prop( SimpleVectorProperty< int >::create( "vec", "int", ints ));
  int* intData( static_cast< int* >( prop->rawData() ));

  ASSERT_TRUE( intData != NULL );
  ASSERT_EQ( prop->elementSize(), sizeof( int ) );

  EXPECT_EQ( intData[ 0 ], 1 );
  EXPECT_EQ( intData[ 1 ], 11 );
  EXPECT_EQ( intData[ 2 ], 111 );
}

TEST_F( VectorPropertyTest, BoolAccessTest )
{
  std::vector< bool > bools;
  VectorPropertyPtr prop( SimpleVectorProperty< bool >::create( "vec", "bool", bools ));

  prop->pushBack();
  prop->pushBack();

  BoolPropertyPtr elt1( dynamic_pointer_cast< BoolProperty >( prop->at( 0 ) ));

  SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
  prop->connect( handler );

  EXPECT_EQ( elt1, elt1->link() );

  EXPECT_FALSE( elt1->get() );
  elt1->set( true );

  EXPECT_TRUE( handler->isUpdated() );

  BoolPropertyPtr elt2( dynamic_pointer_cast< BoolProperty >( prop->at( 1 ) ));
  
  EXPECT_FALSE( elt2->get() );
  elt2->set( true );

  ASSERT_EQ( bools.size(), 2 );
  EXPECT_EQ( bools[ 0 ], true );
  EXPECT_EQ( bools[ 1 ], true );
}

TEST_F( VectorPropertyTest, Vector2fArray )
{
  Array2f vector;
  VectorPropertyPtr prop( SimpleVectorProperty< Vector2f >::create( "array", "vec2f", vector ));
}

TEST_F( VectorPropertyTest, Vector3fArray )
{
  Array3f vector;
  VectorPropertyPtr prop( SimpleVectorProperty< Vector3f >::create( "array", "vec3f", vector ));
}

TEST_F( VectorPropertyTest, Vector4fArray )
{
  Array4f vector;
  VectorPropertyPtr prop( SimpleVectorProperty< Vector4f >::create( "array", "vec4f", vector ));
}

TEST_F( VectorPropertyTest, Vector3iArray )
{
  Array3i vector;
  VectorPropertyPtr prop( SimpleVectorProperty< Vector3i >::create( "array", "vec3i", vector ));
}

TEST_F( VectorPropertyTest, Vector4iArray )
{
  Array4i vector;
  VectorPropertyPtr prop( SimpleVectorProperty< Vector4i >::create( "array", "vec4i", vector ));
}

class MyVectorProperty: public VectorProperty
{
public:
  MyVectorProperty(): VectorProperty( "vector" ) {};

  uint32_t size() const { return 0; };
  PropertyPtr at( uint32_t index ) { return PropertyPtr(); };

  void insert( uint32_t index ) {};
  void remove( uint32_t index ) {};

  void pushBack() {};

  void resize( uint32_t newSize ) {};
};

TEST_F( VectorPropertyTest, TypesRawAccess )
{
  checkRawAccess< int >();
  checkRawAccess< float >();
  checkRawAccess< uint32_t >();
  checkRawAccess< Vector2f >();
  checkRawAccess< Vector3f >();
  checkRawAccess< Vector4f >();
  checkRawAccess< Vector3f >();
  checkRawAccess< Vector3i >();
  checkRawAccess< Vector4i >();
  checkRawAccess< Transform3f >();

  MyVectorProperty property;
  EXPECT_TRUE( property.rawData() == NULL );
  EXPECT_EQ( property.elementSize(), 0 );

  checkNoRawAccess< bool >();
  checkNoRawAccess< string >();
}

