/*
** Cayuga
**
** Error Logging
**
** Oct 2005 ajd
*/

#include <cstdio>
#include <cstdlib>

#include "MemoryMgr/CY_ErrLog.h"

namespace CY_ErrLog {

void die0(const char * fname, int lineno) {
	fprintf(stderr, "ARRGH! Dying at line %d of file %s ...\n", lineno, fname);
	fflush(stderr);
	abort();
}

} /* CY_ErrLog */
