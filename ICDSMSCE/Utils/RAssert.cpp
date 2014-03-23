
#include <cassert>
#include <exception>
using namespace std;

#include "Utils/RAssert.h"

void Cayuga::Utils::Assert(bool b) {
#if _DEBUG 
	assert(b);
#else
	if (!b)
		throw exception();
#endif
}
