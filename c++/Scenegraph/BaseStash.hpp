/*
   -----------------------------------------------------------------------------
   |                 R T R T    S C E N E G R A P H   L I B R A R Y                 |
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

#ifndef RTRT_SCENEGRAPH_BASESTASH_HPP
#define RTRT_SCENEGRAPH_BASESTASH_HPP

#include "Time.hpp"
#include "Stashable.hpp"
#include "AttributeList.hpp"
#include "Node.hpp"
#include <FFIObject.hpp>
#include "BasePropertyHandler.hpp"

namespace rtrt
{
  namespace scenegraph
  {
    /*! Safety casts stashable object to stash interface. */
    template< class Interface >
    static Interface* interfaceCast( BaseStashable* object );

    /*! Safety casts stashable object to base class. */
    template< class Interface >
    static BaseStashable* baseCast( Interface* interface );


    /* Predeclaration of base stash interface. */
    class BaseStashInterface;


    /*! Base class for stashes. */
    class BaseStash : public Timed, burning::FFIObject
    {
    public:
      /*! Creates new abstract stash. */
      BaseStash();
      virtual ~BaseStash() {};

      /*! Adds object to the stash without memory allocation. */
      void addObject( Node* node
                      /*!< node to be added to the stash */ );

      /*! Adds object to the stash without memory allocation. */
      void addObject( Node* node,
                      /*!< node to be added to the stash */
                      BaseStashable* kernel
                      /*!< stashable object within this node */ );

      /*! Removes object from the stash. */
      void removeObject( Node* object
                         /*!< node to be removed from the stash */ );

      /*! Removes object from the stash. */
      void removeObject( Node* node,
                         /*!< node to be removed from the stash */
                         BaseStashable* kernel
                         /*!< stashable object within this node */ );

      /*! Allocates memory for object in the stash. */
      void allocateObject( Node* node
                           /*!< node for which memory is allocated */ );

      /*! Allocates memory for object in the stash. */
      void allocateObject( Node* node,
                           /*!< node for which memory is allocated */
                           BaseStashable* kernel
                           /*!< stashable subkernel within this node */ );

      /*! Releases memory of the object in the stash. */
      void freeObject( Node* node
                       /*!< node for which the memory is released */ );

      /*! Releases memory of the object in the stash. */
      void freeObject( Node* node,
                       /*!< node for which the memory is released */
                       BaseStashable* kernel
                       /*!< stashable subkernel within this node */ );

      /*! Writes object's data to the stash. */
      void writeObject( Node* node,
                        /*!< node to write data to the stash */
                        const AttributeList& list = AttributeList(),
                        /*! list of scenegraph attributes */
                        const Time& time = Time()
                        /*!< inner scenegraph time */ );

      /*! Writes object's data to the stash. */
      void writeObject( Node* node,
                        /*!< node to write data to the stash */
                        BaseStashable* kernel,
                        /*!< stashable subkernel within this node */
                        const AttributeList& list = AttributeList(),
                        /*! list of scenegraph attributes */
                        const Time& time = Time()
                        /*!< inner scenegraph time */ );

      /*! Returns stashing interface of specified object for currect stash. */
      virtual BaseStashInterface* interface( BaseStashable* object ) = 0;

      struct ScenegraphID
      {
        size_t geometryID;
        size_t materialID;
        size_t lightID;
      };

      ScenegraphID& scenegraphID();
      const ScenegraphID& scenegraphID() const;

      const unsigned char* type_uuid() const;
      virtual std::string typeName() const = 0;

    protected:
      /*! Adds object with specified id to the stash */
      virtual void doAddObject( BaseStashable* object, size_t id ) = 0;
      /*! Removes object with specified id from the stash */
      virtual void doRemoveObject( BaseStashable* object, size_t id ) = 0;

      /*! Allocates memory for object with specified id in the stash */
      virtual void doAllocateObject( BaseStashable* object, size_t id ) = 0;
      /*! Releases memory of object with specified id in the stash */
      virtual void doFreeObject( BaseStashable* object, size_t id ) = 0;

      /*! Writes object's data to the stash */
      virtual void doWriteObject( BaseStashable* object, 
				  size_t id, 
				  const AttributeList& attributes,
				  const Time& time ) = 0;

      /*! Map of identifiers for scenegraph nodes */
      typedef std::map< Node* , size_t > StashMap;
      
      /*! Returns indices map for the stash */
      const StashMap& objectIds() const { return _ids; };
      /*! Returns id of specified object in the stash */
      size_t id( Node* object );

    private:
      static BaseStashable* nodeInterface( Node* node );

      ScenegraphID _scenegraphID;
      StashMap _ids;
      
    };

    typedef boost::shared_ptr< BaseStash > BaseStashPtr;

    /*! Base class for stashing interface */
    class BaseStashInterface : public Timed, virtual public Kernel
    {
    public:
      BaseStashInterface();
      virtual ~BaseStashInterface() {};

      /*! Adds object to specified stash */
      void addToStash( BaseStash& stash );
      /*! Removes object from stash */
      void removeFromStash();

      /*! Allocates space for object in stash */
      void allocate();
      /*! Free's object's space in stash */
      void free();

      /*! Set's object new */
      void setNew();

    protected:
      /*! Currect object's stash */
      BaseStash* stash() { return _stash; };

      BasePropertyHandlerPtr _propertyHandler;

    private:
      BaseStashInterface( const BaseStashInterface& stash );
      void operator=( const BaseStashInterface& stash );

      BaseStash* _stash;
    };

    /*! Template class for real stashes */
    template< class InterfaceType >
    class Stash : public BaseStash
    {
    public:
      /*! Type of object's stash interface */
      typedef InterfaceType StashInterface;
      BaseStashInterface* interface( BaseStashable* object ) { return interfaceCast< InterfaceType >( object ); };

    };
  }
}
#include "BaseStash_impl.hpp"
#endif
