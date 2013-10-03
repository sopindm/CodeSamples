#include <gtest/gtest.h>
#include <boost/assign/std/vector.hpp>
#include <Property/AtomProperty.hpp>
#include "SimpleUpdateHandler.hpp"
#include <Property/PropertyBuilder.hpp>

using std::vector;
using std::string;
using namespace rtrt::property;
using namespace boost::assign;

class AtomPropertyTest : public testing::Test
{
public:
  template< class T >
  void checkDefault( T& source, const string& name, boost::shared_ptr< TypedProperty< T > >& property, 
		     const T& value )
  {
    EXPECT_EQ( property->name(), name );

    SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
    property->connect( handler );

    property->setName( "newName" );
    EXPECT_EQ( property->name(), "newName" );

    EXPECT_TRUE( handler->isUpdated() );

    EXPECT_TRUE( property->lowerBound() == NULL );
    EXPECT_TRUE( property->upperBound() == NULL );

    EXPECT_TRUE( property->possibleValues() == vector< T >() );

    EXPECT_EQ( property->get(), value );
    EXPECT_EQ( source, value );
    EXPECT_EQ( property->access(), &source );

    EXPECT_EQ( property->link(), property );
  }

  template< class T >
  void checkValue( T& source, TypedProperty< T >& property, const T& value )
  {
    SimpleUpdateHandlerPtr handler( new SimpleUpdateHandler() );
    SimpleUpdateHandlerPtr handler2( new SimpleUpdateHandler() );

    property.connect( handler );

    property.connect( handler2 );
    property.disconnect( handler2 );

    property.set( value );
    EXPECT_EQ( property.get(), value );
    EXPECT_EQ( source, value );
    EXPECT_TRUE( handler->isUpdated() );
    EXPECT_FALSE( handler2->isUpdated() );
  }

};

TEST_F( AtomPropertyTest, SimpleBoolProperty )
{
  bool b( false );
  BoolPropertyPtr prop( SimpleProperty< bool >::create( "bool", b ));

  checkDefault( b, "bool", prop, false );

  checkValue( b, *prop, true );
  checkValue( b, *prop, false );
}

TEST_F( AtomPropertyTest, SimpleIntProperty )
{
  int i( 0 );
  IntPropertyPtr prop( SimpleProperty< int >::create( "int", i ));

  checkDefault( i, "int", prop, 0 );

  checkValue( i, *prop, 42 );
  checkValue( i, *prop, -123 );
}

TEST_F( AtomPropertyTest, SimpleUIntProperty )
{
  uint i( 0 );
  UIntPropertyPtr prop( SimpleProperty< uint >::create( "uint", i ));

  checkDefault< uint >( i, "uint", prop, 0 );

  checkValue< uint >( i, *prop, 42 );
  checkValue< uint >( i, *prop, 123456 );
}

TEST_F( AtomPropertyTest, SimpleFloatProperty )
{
  float f( 0. );
  FloatPropertyPtr prop( SimpleProperty< float >::create( "float", f ));

  checkDefault< float >( f, "float", prop, 0. );

  checkValue< float >( f, *prop, 1.23 );
  checkValue< float >( f, *prop, 1.23e-46 );
}

TEST_F( AtomPropertyTest, SimpleStringProperty )
{
  string s( "Hello, world" );
  StringPropertyPtr prop( SimpleProperty< string >::create( "string", s ));

  checkDefault< string >( s, "string", prop, "Hello, world" );

  checkValue< string >( s, *prop, "Some string" );
  checkValue< string >( s, *prop, "Hello, properties!!!" );
}

TEST_F( AtomPropertyTest, ReadonlyStringProperty )
{
  string s( "Hello, world" );
  StringPropertyPtr prop( SimpleProperty< string, true >::create( "string", s ));

  EXPECT_EQ( prop->get(), "Hello, world" );

  prop->set( "other string" );
  EXPECT_EQ( s, "Hello, world" );
}

TEST_F( AtomPropertyTest, RangedPropertyTest )
{
  int value( 42 );
  boost::shared_ptr< TypedProperty< int > > prop(  RangedProperty< int >::create( "intRange", value, 0, 100 ) );

  EXPECT_EQ( prop->get(), 42 );

  ASSERT_FALSE( prop->lowerBound() == NULL );
  ASSERT_FALSE( prop->upperBound() == NULL );

  EXPECT_EQ( *prop->lowerBound(), 0 );
  EXPECT_EQ( *prop->upperBound(), 100 );

  prop->set( 99 );
  EXPECT_EQ( prop->get(), 99 );

  prop->set( -1 );
  EXPECT_EQ( prop->get(), 99 );

  prop->set( 102 );
  EXPECT_EQ( prop->get(), 99 );

  EXPECT_EQ( prop->link(), prop );
}

TEST_F( AtomPropertyTest, ListedValiuePropertyTest )
{
  int value( 42 );

  vector< int > values;
  values += 0, 5, 10, 20, 40, 42, 50, 100;

  boost::shared_ptr< TypedProperty< int > > prop( ListedValueProperty< int >::create( "list", value, values ) );

  EXPECT_EQ( prop->get(), 42 );

  EXPECT_TRUE( prop->lowerBound() == NULL );
  EXPECT_TRUE( prop->upperBound() == NULL );

  EXPECT_TRUE( prop->possibleValues() == values );

  prop->set( 5 );
  EXPECT_EQ( prop->get(), 5 );

  prop->set( 50 );
  EXPECT_EQ( prop->get(), 50 );

  prop->set( -111 );
  EXPECT_EQ( prop->get(), 50 );

  EXPECT_EQ( prop->link(), prop );
}
