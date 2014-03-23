/**
 * @file StringMap.h
 * @date November 26, 2005
 * @brief Defines the StringMap type.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _STRING_MAP_H
#define _STRING_MAP_H

#include "arch-dep.h"

#include <string>
using namespace std;

namespace Cayuga {
namespace Utils {
typedef hash_map<string, string> StringMap;
}  //namespace Utils
}  //namespace Cayuga

#endif //_STRING_MAP_H
