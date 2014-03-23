/**
 * @file Split.cpp
 * @date Feburary 22, 2005
 * @brief Defines the function split below.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <vector>
#include <string>
using namespace std;

#include "Utils/split.h"

namespace Cayuga {
namespace Utils {
void split(const string& input,
		   vector<string>& output,
		   char delim) {
   if (input == "") return;

   string::size_type start = 0;

   while (true) {
	   string::size_type pos 
		   = input.find_first_of(delim, start);
	   string field = input.substr(start, pos-start);
	   output.push_back(field);
	   if (pos == string::npos)
		   return;

	   start = pos+1;
	   while (input[start] == ' ')
		   start++;
   }
}
}
}
