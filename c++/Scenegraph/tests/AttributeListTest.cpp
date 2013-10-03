#include <gtest/gtest.h>
#include <Scenegraph/AttributeList.hpp>

using namespace Eigen;
using namespace rtrt::scenegraph;

class AttributeListTest : public testing::Test
{
public:
  static void checkVector( const Vector3f& vec1, const Vector3f& vec2 )
  {
    EXPECT_FLOAT_EQ( vec1.x(), vec2.x() );
    EXPECT_FLOAT_EQ( vec1.y(), vec2.y() );
    EXPECT_FLOAT_EQ( vec1.z(), vec2.z() );
  }
};

TEST_F( AttributeListTest, SimpleList )
{
  AttributeList list;
  list.setAttribute( "Visibility", VisibilityAttribute( false ) );

  VisibilityAttributePtr attr = SCENEGRAPH_ATTRIBUTE( Visibility, list );
  ASSERT_TRUE( attr != NULL );

  EXPECT_FALSE( attr->value() );
}

TEST_F( AttributeListTest, ReadWrite )
{
  AttributeList list;
  
  SCENEGRAPH_SET_ATTRIBUTE( VisibilityAttribute(), Visibility, list );
  SCENEGRAPH_SET_ATTRIBUTE( static_cast< Transform3f >( Translation3f( 1.0, 2.0, 3.0 )), Transform, list );

  Time time( 213 );

  VisibilityAttributePtr attr1 = SCENEGRAPH_ATTRIBUTE( Visibility, list );
  ASSERT_TRUE( attr1 != NULL );
  EXPECT_TRUE( attr1->value() );
  EXPECT_TRUE( attr1->isOld( time ) );

  TransformAttributePtr attr2 = SCENEGRAPH_ATTRIBUTE( Transform, list );
  ASSERT_TRUE( attr2 != NULL );
  checkVector( attr2->value() * Vector3f( 3.0, 2.0, 1.0 ), Vector3f( 4.0, 4.0, 4.0 ) );
  EXPECT_TRUE( attr2->isNew( time ) );

  SCENEGRAPH_SET_ATTRIBUTE( false, Visibility, list );

  attr1 = SCENEGRAPH_ATTRIBUTE( Visibility, list );
  ASSERT_TRUE( attr1 != NULL );
  EXPECT_FALSE( attr1->value() );
  EXPECT_TRUE( attr1->isNew( time ) );

  SCENEGRAPH_SET_ATTRIBUTE( static_cast< Transform3f >( Scaling3f( 2.0, 3.0, 0.5 ) ), Transform, list );

  attr2 = SCENEGRAPH_ATTRIBUTE( Transform, list );
  ASSERT_TRUE( attr2 != NULL );
  checkVector( attr2->value() * Vector3f( 1.0, 1.0, 1.0 ), Vector3f( 2.0, 3.0, 0.5 ) );
  EXPECT_TRUE( attr2->isNew( time ));
}

TEST_F( AttributeListTest, CopyMoveTest )
{
  AttributeList list;

  SCENEGRAPH_SET_ATTRIBUTE( VisibilityAttribute( false ), Visibility, list );
  SCENEGRAPH_SET_ATTRIBUTE( static_cast< Transform3f >( Translation3f( 1.0, 2.0, 3.0 )), Transform, list );

  AttributeList list2( list );

  VisibilityAttributePtr vAttr( SCENEGRAPH_ATTRIBUTE( Visibility, list2 ));
  ASSERT_TRUE( vAttr != NULL );
  EXPECT_FALSE( vAttr->value() );

  vAttr->setValue( true );

  vAttr = SCENEGRAPH_ATTRIBUTE( Visibility, list );
  EXPECT_FALSE( vAttr->value() );

  AttributeList list3;
  list3 = list2;

  TransformAttributePtr tAttr( SCENEGRAPH_ATTRIBUTE( Transform, list3 ));
  ASSERT_TRUE( tAttr != NULL );
  EXPECT_TRUE( tAttr->value().isApprox( static_cast< Transform3f >( Translation3f( 1.0, 2.0, 3.0 ))) );
}

TEST_F( AttributeListTest, DefaultValueTest )
{
  AttributeList list;

  TransformAttributePtr attr( SCENEGRAPH_ATTRIBUTE( Transform, list ));

  ASSERT_TRUE( attr != NULL );
  EXPECT_TRUE( attr->value().isApprox( TransformAttribute().value() ));
}

TEST_F( AttributeListTest, MergeTest )
{
  AttributeList list1;
  SCENEGRAPH_SET_ATTRIBUTE( false, Visibility, list1 );

  AttributeList list2;
  SCENEGRAPH_SET_ATTRIBUTE( true, Visibility, list2 );

  AttributeList list3 = list1.merge( list2 );

  EXPECT_FALSE( SCENEGRAPH_ATTRIBUTE( Visibility, list3 )->value() );
}
