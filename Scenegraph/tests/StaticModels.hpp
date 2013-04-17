#ifndef RTRT_SCENEGRAPHTEST_STATICMODELS_HPP
#define RTRT_SCENEGRAPHTEST_STATICMODELS_HPP

#include <Scenegraph/StaticModel.hpp>
#include <Scenegraph/Node.hpp>

using Eigen::Vector3f;
using namespace rtrt::scenegraph;
using namespace rtrt::scenegraph::modelStash;

class StaticModels
{
public:
  void setup();

  StaticModelPtr model1, model2, model3;
  NodePtr node1, node2, node3;

  ModelStashPtr stash;

  void makeModel1();
  void makeModel2();
  void makeModel3();
  
  void checkStash();
  void checkStash( const StaticModel& model , size_t index1 = 0 );
  void checkStash( const StaticModel& model1, const StaticModel& model2, size_t index1 = 0, size_t index2 = 1 );
  void checkStash( const StaticModel& model1, const StaticModel& model2, const StaticModel& model3 );

  template< class T1, class T2 >
  void checkArray3f( const T1& v1, const T2& v2 )
  {
    ASSERT_EQ( v1.size(), v2.size() );

    for( size_t i=0; i<v1.size(); i++ )
    {
      EXPECT_FLOAT_EQ( v1[ i ].x(), v2[ i ].x() );
      EXPECT_FLOAT_EQ( v1[ i ].y(), v2[ i ].y() );
      EXPECT_FLOAT_EQ( v1[ i ].z(), v2[ i ].z() );
    }
  }

};

template< class T1, class T2 >
static std::vector< T1, T2 > operator+( const std::vector< T1, T2 >& v1, const std::vector< T1, T2 >& v2 )
{
  std::vector< T1, T2 > ret( v1 );

  for( size_t i=0; i<v2.size(); i++ )
    ret.push_back( v2[ i ] );

  return ret;
}

#endif
