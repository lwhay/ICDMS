/**
 * @file PredIndexSpecString.h
 * @date November 20, 2005
 * @brief Stores the information of a predicate index spec read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _PRED_INDEX_SPEC_H
#define _PRED_INDEX_SPEC_H


#include "BasicDataStructure/Predicate/AttributeID.h"
using Cayuga::BasicDataStructure::Predicate::AttributeID;

#include "BasicDataStructure/StreamID.h"
using Cayuga::BasicDataStructure::StreamID;

#include "Utils/Singleton.h"
using Cayuga::Utils::Singleton;

#include <cassert>
#include <string>
#include <vector>
#include <iostream>
using namespace std;
#include <string.h>

namespace Cayuga {
namespace AutomatonCompiler {
class PredIndexSpecString;
class Context;
class NormalAtomPredString;

/**
 * @brief Type of a predicate index
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 20, 2005
 * @version 1.0
 */
enum IndexType {
	HASH,
	B
};

/**
 * @brief A PredIndexSpecEntryString object represents one entry in 
		  a predicate index spec constructed from SAX events.	
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 20, 2005
 * @version 1.0
 */
class PredIndexSpecEntryString {
	friend class PredIndexSpecString;
public:
	/**
	* @brief Default constructor
	* @param indexType Type of this index spec entry. Could be STATIC or DYNAMIC
	* @param evAttr Event attribute which the index works on
	* @param nodeAttr Node attribute which the index works on. Not empty only 
	                  if this is a dynamic pred index entry.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	PredIndexSpecEntryString (const string& indexType,
		const string&  evAttr,
		const string&  nodeAttr 
		);

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this predicate
			 index spec entry.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief Returns true iff this index spec does work on the input event 
			 attribute.
	* @param eventAttr The string name of the event attribute
	* @return As described above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	bool indexIsOnEventAttr(const string& eventAttr) const;

	/**
	* @brief Convert event attribute name of string format to internal integer
		     by looking at symbol table.
	* @param streamID The ID of the input stream the predicate index works on
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	AttributeID eventAttrName2AttrID(const StreamID& streamID) const;

	/**
	* @brief Returns the Node Attribute part of the Index Spec Entry
	* @return The string with Node Attribute
	* @author Varun Sharma (mshong@cs.cornell.edu)
	* @date June 25, 2006
	* @version 1.0
	*/
	string getNodeAttr() const;

private:
	/** 
	* This variable stores the type of the index, indicating whether the 
	* index of this attribute is static or dynamic.
	*/
	string _indexType;

	/** 
	* This variable stores the event attribute on which the index works.
	*/
	string _evAttr;
	
	/** 
	* This variable stores the node attribute on which the index works. 
	* It is not empty only if this is a dynamic index spec.
	*/
	string _nodeAttr;
}; //class PredIndexSpecEntryString

/**
 * @brief A PredIndexSpecString object represents a predicate index spec 
		  constructed from SAX events.	
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 20, 2005
 * @version 1.0
 */
class PredIndexSpecString :	public Singleton<PredIndexSpecString>{
public:

	typedef vector<PredIndexSpecEntryString> PredIndexSpecEntrySet;
		
	/**
	* @brief Compiles this predicate index spec into Cayuga engine.
	* @param context The compilation context
	* @param indexInputLengthOut Set by the function to the input length of 
	         this indexed attribute
	* @return Pointer to the internalized index spec.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	char* compile(const Context& context,
		size_t& indexInputLengthOut) const;

	/**
	* @brief Set the index type of this spec.
	* @param indexType Type of the index to set.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	void setIndexType (const string& indexType);

	/**
	* @brief Append the predicate index spec entry.
	* @param entry The index spec entry to append.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	void push_back(const PredIndexSpecEntryString& entry);

	/**
	* @brief Clear all entries in this index spec.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/	
	void clear();

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this predicate
			 index spec.
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	void write(ostream& out = cout) const;

	/**
	* @brief See whether the input event attribute is indexed here.
	* @param eventAttr The event attribute to look up
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	bool find(const string& eventAttr) const;

	/**
	* @brief See whether the input (event,node) pair is indexed here.
	* @param eventAttr The event attribute to look up
	* @param nodeAttr The node attribute to look up
	* @author Varun Sharma (varun13687@yahoo.com)
	* @date June 25, 2006
	* @version 1.0
	*/
	bool find(const string& eventAttr, const string& nodeAttr) const;


	/**
	* @brief See whether the index spec matches the input predicate
	* @param indexedPreds The predicate to check
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	bool specMatchesPred(
		const vector<NormalAtomPredString*>& indexedPreds) const;

	/**
	* @brief Convert this index spec to a sequence of event attribute IDs.
	* @param context The compilation context
	* @return A sequence of event attribute IDs 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 20, 2005
	* @version 1.0
	*/
	vector<AttributeID> spec2InternalIndexSpec(
	    const Cayuga::AutomatonCompiler::Context& context) const;
	
	/**
	* @brief Convert this Dynamic index spec to a sequence of pairs of 
	*		 event attribute IDs and Node Attribute IDs
	* @param context The compilation context
	* @return A sequence of (event attribute IDs, node attribute IDs)
	* @author Varun Sharma (mshong@cs.cornell.edu)
	* @date June 25, 2005
	* @version 1.0
	*/
	vector<pair<AttributeID, AttributeID> > spec2InternalDynIndexSpec
					(const Context& context) const;

private:
	/** 
	* This variable indicates whether this index is HASH index or B tree index.
	*/
	string _indexType;
	
	/** 
	* This variable stores the set of index spec entries in this spec.
	*/
	PredIndexSpecEntrySet _predIndexEntries;
}; //class PredIndexSpecString
}  //namespace AutomatonCompiler
}  //namespace Cayuga


#endif //_PRED_INDEX_SPEC_H
