#include <gtest/gtest.h>
#include <boost/assign/std/vector.hpp>
#include <Property/AtomProperty.hpp>
#include <Property/ListProperty.hpp>
#include <Property/PropertyBuilder.hpp>
#include <Eigen/Core>
#include <Core/Types.hpp>

using namespace Eigen;
using std::vector;
using std::string;
using namespace rtrt;
using namespace rtrt::property;
using namespace boost::assign;
using namespace boost;

class ComplexPropertiesTest : public testing::Test
{
public:
  template< class T >
  void checkComponent( PropertyPtr property, const std::string& name, T value, T& source );

  void checkTransformRow( PropertyPtr rowProperty, const std::string& name, 
			  float at0, float at1, float at2, float at3, 
			  Transform3f& source, int index );

};

template< class T >
void ComplexPropertiesTest::checkComponent( PropertyPtr property, 
					    const std::string& name, 
					    T value,
					    T& source )
{
  boost::shared_ptr< TypedProperty< T > > elt = Property::cast< TypedProperty< T > >( property );

  EXPECT_EQ( elt->name(), name );
  EXPECT_EQ( elt->get(), value );

  elt->set( value * 2 + 1 );
  EXPECT_EQ( source, value * 2 + 1 );
}

void ComplexPropertiesTest::checkTransformRow( PropertyPtr row, const string& name,
					       float at0, float at1, float at2, float at3, 
					       Transform3f& source, int index )
{
  ListPropertyPtr prop = Property::cast< ListProperty >( row );

  EXPECT_EQ( prop->name(), name );
  ASSERT_EQ( prop->size(), 4 );

  checkComponent( prop->at( 0 ), "x", at0, source( index, 0 ) );
  checkComponent( prop->at( 1 ), "y", at1, source( index, 1 ) );
  checkComponent( prop->at( 2 ), "z", at2, source( index, 2 ) );
  checkComponent( prop->at( 3 ), "w", at3, source( index, 3 ) );
}

TEST_F( ComplexPropertiesTest, Vector2fProperty )
{
  Vector2f vector( 1., 2. );

  ListPropertyPtr prop( Property::cast< ListProperty >( buildProperty( "vector", vector )));

  ASSERT_EQ( prop->size(), 2 );

  checkComponent( prop->at( 0 ), "x", 1.F, vector.x() );
  checkComponent( prop->at( 1 ), "y", 2.F, vector.y() );
}

TEST_F( ComplexPropertiesTest, Vector3fProperty )
{
  Vector3f vector( 1., 2., 3. );

  ListPropertyPtr prop( Property::cast< ListProperty >( buildProperty( "vector", vector )));

  ASSERT_EQ( prop->size(), 3 );

  checkComponent( prop->at( 0 ), "x", 1.F, vector.x() );
  checkComponent( prop->at( 1 ), "y", 2.F, vector.y() );
  checkComponent( prop->at( 2 ), "z", 3.F, vector.z() );
}

TEST_F( ComplexPropertiesTest, Vector4fProperty )
{
  Vector4f vector( 1., 2., 3., 4 );

  ListPropertyPtr prop( Property::cast< ListProperty >( buildProperty( "vector", vector )));

  ASSERT_EQ( prop->size(), 4 );

  checkComponent( prop->at( 0 ), "x", 1.F, vector.x() );
  checkComponent( prop->at( 1 ), "y", 2.F, vector.y() );
  checkComponent( prop->at( 2 ), "z", 3.F, vector.z() );
  checkComponent( prop->at( 3 ), "w", 4.F, vector.w() );
}

TEST_F( ComplexPropertiesTest, Transform3fProperty )
{
  Transform3f transform( Transform3f::Identity() );
  transform *= Translation3f( 1., 2., 3. );

  ListPropertyPtr prop( Property::cast< ListProperty >( buildProperty( "transform", transform )));

  ASSERT_EQ( prop->size(), 4 );

  checkTransformRow( prop->at( 0 ), "row1", 1, 0, 0, 1, transform, 0 );
  checkTransformRow( prop->at( 1 ), "row2", 0, 1, 0, 2, transform, 1 );
  checkTransformRow( prop->at( 2 ), "row3", 0, 0, 1, 3, transform, 2 );
  checkTransformRow( prop->at( 3 ), "row4", 0, 0, 0, 1, transform, 3 );
}

TEST_F( ComplexPropertiesTest, Vector3iProperty )
{
  Vector3i vector( 1, 2, 3 );

  ListPropertyPtr prop( Property::cast< ListProperty >( buildProperty( "vector", vector )));

  ASSERT_EQ( prop->size(), 3 );

  checkComponent( prop->at( 0 ), "v1", 1, vector.x() );
  checkComponent( prop->at( 1 ), "v2", 2, vector.y() );
  checkComponent( prop->at( 2 ), "v3", 3, vector.z() );
}

TEST_F( ComplexPropertiesTest, Vector4iProperty )
{
  Vector4i vector( 1, 2, 3, 4 );

  ListPropertyPtr prop( Property::cast< ListProperty >( buildProperty( "vector", vector )));

  ASSERT_EQ( prop->size(), 4 );

  checkComponent( prop->at( 0 ), "v1", 1, vector.x() );
  checkComponent( prop->at( 1 ), "v2", 2, vector.y() );
  checkComponent( prop->at( 2 ), "v3", 3, vector.z() );
  checkComponent( prop->at( 3 ), "v4", 4, vector.w() );
}

