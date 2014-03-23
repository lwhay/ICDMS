/**
 * @file Singleton.h
 * @date November 17, 2005
 * @brief Defines an abstract class to make it easy to construct
          a concrete singleton class by inheriting it.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _SINGLETON_H
#define _SINGLETON_H

#include <cstdio>
using namespace std;

namespace Cayuga {
namespace Utils {
/**
 * @brief A Singleton object represents an object that cannot have two instances
		  in the system.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 17, 2005
 * @version 1.0
 */
template <typename T>
class Singleton  {
public:
	/**
	* @brief Used to get the singleton object.
	* @return Return the singleton object.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	static T* getInstance();

protected:
	/**
	* @brief Default constructor.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	Singleton() {}
	
	/**
	* @brief Virtual destructor.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 17, 2005
	* @version 1.0
	*/
	virtual ~Singleton() {
		delete _singleton;
	}

private:
	/**
	* This variable stores the pointer to the singleton object.
	*/
	static T* _singleton;
};


template <typename T>
T* Singleton<T>::_singleton = NULL;

template <typename T>
T* Singleton<T>::getInstance() {
	if (_singleton == NULL) {
		_singleton = ::new T();
	}
	return _singleton;
}  //class Singleton
}  //namespace Utils
}  //namespace Cayuga

#endif //_SINGLETON_H
