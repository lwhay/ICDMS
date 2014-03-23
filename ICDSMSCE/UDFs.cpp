/**
 * @file UDFs.cpp
 * @date April 4, 2007
 * @brief This file defines the user-defined functions loaded into Cayuga.
			It is copied from the old init.cpp.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include "../ICDSMSCE/MemoryMgr/CY_IntStringFace.h"
using namespace CY_IntString;

#include "../ICDSMSCE/SystemMgr/FuncMgr.h"
using namespace Cayuga::SystemMgr;

#include <algorithm>
using namespace std;

/**
 * @brief An TextMatcher object calculates the similarity of two input strings.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date April 20, 2007
 * @version 1.0
 */
class TextMatcher {
public:
	typedef vector<string> KGramType;

	/**
	* @brief Default constructor.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 20, 2007
	* @version 1.0
	*/
	TextMatcher() : K(10), P(1) {
	}
	
	/**
	* @brief Return a measure between 0 and 1 on how close similar two
			 input strings are.
	* @param lhs The first input string
	* @param rhs The second input string
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 20, 2007
	* @version 1.0
	*/
	float similarity(IntStringHandle& lhs, IntStringHandle& rhs) {
		//cout<<"lhs is "<<lhs->getBody()<<endl;
		//cout<<"rhs is "<<rhs->getBody()<<endl;

		KGramType lhsKGram;
		assert (lhs->getBody());
		getKgrams(lhs->getBody(), lhsKGram);

		KGramType rhsKGram;
		assert (rhs->getBody());
		getKgrams(rhs->getBody(), rhsKGram);

		size_t retSize = max(lhsKGram.size(), rhsKGram.size());
		KGramType tmp_hs(retSize);
		KGramType::iterator result_it 
			= getIntersection(lhsKGram, rhsKGram, tmp_hs);
		tmp_hs.erase(result_it, tmp_hs.end());

		//HashCode temp = (KGramType)one.clone();
		//temp.retainAll(two);
		long min_size = min((long)lhsKGram.size(), (long)rhsKGram.size());
		return (float)((tmp_hs.size()) / (double)min_size);
	}

private:
	/*
	* If this turns out to be too slow, (which it very well might be), I would recommend switching to a
	* "rolling" implementation, so that the entire hash does not need to be
	* recomputed when the string is only shifted by one char.
	* returns the pointer to the retVal
	*/
	void getKgrams(const string& s, KGramType& retval) {
		if ((long)s.size() < K) return;

		long hashCode = 0; 
		for (long bitIdx = 0; bitIdx < K; bitIdx++) {
			hashCode += (hashCode<<SHIFT_BIT) + s[bitIdx];
		}
		hashCode = hashCode<<SHIFT_BIT;
		if (hashCode % P == 0) {
			//retval.insert(s.substr(0, K));
			retval.push_back(s.substr(0, K));
		}

		//long b_to_the_k = 1 << (SHIFT_BIT*K); //b^k

		hashCode -= (s[0]<<(SHIFT_BIT*K));
		for (long index = 1; index + K <= (long)s.size(); index++) {
			string temp = s.substr(index, K);

			hashCode = (hashCode + temp[K-1]) << SHIFT_BIT;
			if (hashCode % P == 0) {
				//retval.insert(temp);
				retval.push_back(temp);
			}

			hashCode -= (temp[0]<<(SHIFT_BIT*K));
		}
	}


	KGramType::iterator getIntersection(KGramType& left, 
		KGramType& right, 
		KGramType& outSet) {

		sort ( left.begin ( ) , left.end ( ) );
		sort ( right.begin ( ) , right.end ( ));

		KGramType::iterator result_it 
			= set_intersection ( left.begin ( ) , left.end ( ) ,
			right.begin ( ) , right.end ( ) , outSet.begin ( ) );
		
		//for (KGramType::iterator Iter1 = outSet.begin( ) ; Iter1 != result_it ; Iter1++ )
		//	cout << *Iter1 << " ";
		//cout << endl;
		return result_it;
	}

	//text matching algorithm parameters
	long K; 
	long P;
	
	static const int SHIFT_BIT = 1;
};

/**
* @brief Given two input strings, calculuate a similarity measure.
* @param arg1 The first input string. To be cast into IntStringHandle*
* @param arg2 The second input string. To be cast into IntStringHandle*
* @param ret To store the output similarity value. To be cast into float* 
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 20, 2007
* @version 1.0
*/
void similarity(void* arg1, void* arg2, void* ret) {
	//return 1.0;
	TextMatcher tm;
	*(float*)ret = tm.similarity(*(IntStringHandle*)arg1, 
		*(IntStringHandle*)arg2);
}

/**
* @brief Given two input strings, 
		 answer whether the first one contains the second.
* @param arg1 The first input string (text). To be cast into IntStringHandle*
* @param arg2 The second input string (pattern). 
		 To be cast into IntStringHandle*
* @param ret To store the boolean answer of whether text contains pattern. 
		 1 if so, and 0 otherwise. To be cast into int* 
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 20, 2007
* @version 1.0
*/
void contains(void* arg1, void* arg2, void* ret) {
	const char* text = (*(IntStringHandle*)arg1)->getBody();
	const char* pattern = (*(IntStringHandle*)arg2)->getBody();

	assert (text && pattern);

	string strText(text);
	string strPattern(pattern);

	basic_string <char>::size_type retPos = strText.find(strPattern);
	if (retPos != string::npos ) {
		*(int*)ret = 1;
	}
	else {
		*(int*)ret = 0;
	}
}

/**
* @brief Insert the above UDFs into Cayuga.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
* @date April 20, 2007
* @version 1.0
*/
void insertUDFs() {
	FuncMgr::getInstance()->insertFuncEntry("SIMILARITY", 
		ByteCodeNS::FLOAT,
		&similarity,
		2,
		ByteCodeNS::STRING,
		ByteCodeNS::STRING);

	FuncMgr::getInstance()->insertFuncEntry("CONTAINS", 
		ByteCodeNS::INT,
		&contains,
		2,
		ByteCodeNS::STRING,
		ByteCodeNS::STRING);
}
