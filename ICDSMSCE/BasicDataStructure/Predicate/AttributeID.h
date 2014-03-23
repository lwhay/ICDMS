/**
 * @file AttributeID.h
 * @date November 14, 2005
 * @brief Definition of an attribute ID. Put in a separate file to make it easy 
          for other files to include it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _ATTRIBUTE_ID_H
#define _ATTRIBUTE_ID_H

#include <cstdio>

namespace Cayuga {
namespace BasicDataStructure {
namespace Predicate {
typedef std::size_t AttributeID;

typedef unsigned AttrTypeID;

}  //namespace Predicate
}  //namespace BasicDataStructure
}  //namespace Cayuga



#endif //_ATTRIBUTE_ID_H
