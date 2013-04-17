/*
   -----------------------------------------------------------------------------
   |                 R T R T    X M L   L I B R A R Y                 |
   -----------------------------------------------------------------------------
                              
                             
   Copyright (c) 2010 - 2012  Dmitry Sopin ( sopindm @ gmail.com )

   This library is free software: you can redistribute it and/or modify it under
   the terms of the GNU General Public License as published by the Free Software
   Foundation, either version 3 of the License, or (at your option) any later
   version.

   This library is distributed in the hope that it will be useful, but WITHOUT
   ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
   FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License along with
   this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RTRT_XML_XMLNODESPROXY_HPP
#define RTRT_XML_XMLNODESPROXY_HPP

#include <set>
#include <utility>
#include <boost/tr1/memory.hpp>

namespace rtrt
{
  class XmlNode;
  
  template< class T >
  /*! Compares values using their name methods. */
  class LessName
  {
  public:
    /*! A less name operator */
    bool operator()( const T& left, const T& right )
    {
      return left->name() < right->name();
    }
  };
  
  typedef std::tr1::shared_ptr< XmlNode > XmlNodePtr;
  typedef std::multiset< XmlNodePtr, LessName< XmlNodePtr > > NodeSet;
  
  /*! A proxy class for accessing XmlNode values from set */
  class XmlNodesProxy
  {
  public:
    /*! Iterator's range for NodeSet */
    typedef std::pair< NodeSet::iterator, NodeSet::iterator > NodeRange;
    /*! Const iterator's range for NodeSet */
    typedef std::pair< NodeSet::const_iterator, NodeSet::const_iterator > ConstNodeRange;
    
    /*! Constructs proxy from specified iterator's range */
    XmlNodesProxy( const NodeRange& range );
    
    /*! Checks that proxy representing an existing iterator's range */
    bool exist() const;
    /*! Count of nodes in a representing range. */
    size_t count() const;
    
    /*! Iterator's type for representing range.*/
    typedef NodeSet::iterator iterator;
    /*! Const iterator's type for representing range.*/
    typedef NodeSet::const_iterator const_iterator;
    
    /*! Begin of representing iterator's range */
    iterator begin();
    /*! End of representing iterator's range */
    iterator end();
    
    /*! Begin of representing iterator's range */
    const_iterator begin() const;
    /*! End of representing iterator's range */
    const_iterator end() const;
    
  private:
    NodeRange _range;
    ConstNodeRange _constRange;
    
  };
  
  /*! A nodes proxy supporting insert operation */
  class InsertableXmlNodesProxy : public XmlNodesProxy
  {
  public:
    /*! Constructs proxy using range and node object */
    InsertableXmlNodesProxy( const NodeRange& range, XmlNode& node );
    /*! Adds node to collection */
    void operator+=( const XmlNodePtr& node );
    /*! Removes node from collection */
    void operator-=( const XmlNodePtr& node );
    
  private:
    XmlNode& _node;
    
  };
}
#endif
