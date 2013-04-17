#include <gtest/gtest.h>
#include <Scenegraph/Attribute.hpp>
#include <Eigen/Core>

using namespace Eigen;
using namespace rtrt::scenegraph;

class SimpleAttribute : public Attribute
{
public:
  SimpleAttribute(): Attribute( Time( Time::oldest )), value( false )
  {
  }

  SimpleAttribute( bool isNew ): 
    Attribute( Time( Time::oldest )), 
    value( isNew )
  {
    if( isNew )
      setNew();
  }

  AttributePtr clone () const { return AttributePtr( new SimpleAttribute( value )); };
  AttributePtr defaultValue() const { return AttributePtr( new SimpleAttribute() ); };

  bool value;
};

class AttributeTest : public testing::Test
{
public:
  static void mergeCheck( bool state1, bool state2, bool result )
  {
    AttributePtr attr1( new SimpleAttribute( state1 ));
    AttributePtr attr2( new SimpleAttribute( state2 ));

    Time time( 213 );
    attr2->merge( *attr1 );

    if( result )
    {
      EXPECT_TRUE( attr2->isNew( time ) );
    }
    else
    {
      EXPECT_TRUE( attr2->isOld( time ) );
    }
  }
};

TEST_F( AttributeTest, MergeTest )
{
  mergeCheck( false, false, false );
  mergeCheck( true, false, true );
  mergeCheck( false, true, true );
  mergeCheck( true, true, true );
}

class TransformAttributeTest : public testing::Test
{
public:
  void vectorCheck( Vector3f source, Vector3f result )
  {
    EXPECT_FLOAT_EQ( source.x(), result.x() );
    EXPECT_FLOAT_EQ( source.y(), result.y() );
    EXPECT_FLOAT_EQ( source.z(), result.z() );
  }
};

TEST_F( TransformAttributeTest, InitializationTest )
{
  TransformAttribute attr;

  EXPECT_TRUE( attr.value().isApprox( static_cast< Transform3f >(Transform3f::Identity() )));
}

TEST_F( TransformAttributeTest, Merging )
{
  AttributePtr attr1( new TransformAttribute( Transform3f( Translation3f( 1.0, 2.0, 3.0 ))));
  TransformAttribute attr2( Transform3f( Scaling3f( 2.0, 2.0, 2.0 )));

  attr2.merge( *attr1 );

  EXPECT_TRUE( attr2.value().isApprox( Scaling3f( 2.0, 2.0, 2.0 ) * Translation3f( 1.0, 2.0, 3.0 ) ));

  Vector3f sample( 1.0, 1.0, 1.0 );
  vectorCheck( (attr2.value() * sample) , Vector3f( 4., 6., 8. ) );
}

TEST_F( TransformAttributeTest, ChangingTest )
{
  Time time( 213 );

  TransformAttribute attr;
  EXPECT_TRUE( attr.isOld( time ));

  attr.setValue( Transform3f( Scaling3f( 1.0, 2.0, 3.0 )) );
  EXPECT_TRUE( attr.isNew( time ));

  TransformAttribute newAttr( Transform3f( Scaling3f( 1.0, 2.0, 3.0 )));
  EXPECT_TRUE( newAttr.isNew( time ));
}
