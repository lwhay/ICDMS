/**
 * @file IntermediateNodeString.h
 * @date November 27, 2005
 * @brief Stores the information of an intermediate or end node read from AIR.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#ifndef _INTERMEDIATE_NODE_STRING_H
#define _INTERMEDIATE_NODE_STRING_H

#include "StartNodeString.h"
#include "predWindowString.h"
#include "BasicDataStructure/Window/predWindow.h"

namespace Cayuga {
namespace AutomatonCompiler {
/**
 * @brief An IntermediateNodeString object represents an intermediate or end 
		  node constructed from SAX events.	
 * @see	NodeString
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 * @date November 27, 2005
 * @version 1.0
 */
class IntermediateNodeString : public StartNodeString {
public:
	/**
	* @brief Default constructor
	* @param attrMap Stores the attribute name, value pairs associated with the
					 SAX start element event of the node element in AIR. In
					 particular, NodeID is required to occur, and 
					 InputStreamName is optional.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	IntermediateNodeString(const StringMap& attrMap);

	/**
	* @brief Virtual destructor
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual ~IntermediateNodeString();

	/**
	* @brief Compiles this node into Cayuga engine, by translating
			 strings into integers with symbol tables, and so on.
	* @param context The compilation context
	* @return Pointer to the internalized node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual Node* compile(Context& context);

	/**
	* @brief Output member variable information in XML format, aiming at 
			 reconstructing the XML piece in AIR which encodes this node
	* @param out The output stream handle.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void write(ostream& out = cout);	

	/**
	* @brief Add an attribute name, type pair to the schema of this node.
	* @param attrName Name of the attribute.
	* @param attrType Type of the attribute.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addSchemaAttr(const string& attrName,
		const string& attrType);

	/**
	* @brief Add an output stream to this node.
	* @param outputStream Name of the output stream.
	* @param isOut whether to be output to disc/Network or not.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date November 27, 2005
	* @version 1.0
	*/
	virtual void addOutputStream(const string& outputStream,
							    const bool isOut);

	/**
	* @brief Clear the output streams of this node
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 18, 2007
	* @version 1.0
	*/
	void clearOutputStreams();

	/**
	* @brief Return the singleton output stream entry of this node
	* @return As above.
	* @invariant This node must have only one output stream 
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date April 19, 2007
	* @version 1.0
	*/
	const pair<string, bool> getOutputStream();

	/**
	* @brief Get the node schema.
	* @return As above.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 11, 2006
	* @version 1.0
	*/
	virtual StreamSchema* getNodeSchema() const;

	/**
	* @brief Set the node schema.
	* @param pSchema The schema to set to. This class instance now owns the 
			 schema. So there is no copying involved.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 17, 2006
	* @version 1.0
	*/
	void setNodeSchema(StreamSchema* pSchema);

	//luoluo 8-1
	void setRationalPredWindow(predWindowString * ptr);
	//luoluo 8-2
	predWindowString * returnPredWindowString();
// 	//let the predwindowstring into the predwindow.
// 	predWindow*  predwindow_compile();
	//luoluo 8-2
	//whether the intermediate node is the end node 
	bool whether_endNode();
protected:
	/**
	* @brief Construct a node with the input sid. 
			 Refactored existing code to create this makeNode virtual function.
	* @param sid The input stream ID for the new node.
	* @return Point to the newly constructed node.
	* @author Mingsheng Hong (mshong@cs.cornell.edu)
	* @date September 11, 2006
	* @version 1.0
	*/
	virtual Node* makeNode(const StreamID sid);

	/**
	* This variable stores a set of output streams of this node. 

	  Non-empty only if this is an end node for some query. For each output
	  stream, mark it is resubscribed to.
	*/

	/* MODIFICATION : It now also stores the flag whether the stream
	   needs to be output to the disk/network or not
	   date June 16, 2006 
	*/
	vector<pair<string, bool> > _outputStreams;

	
	/** 
	* This variable stores the schema of this node.
	*/
	StreamSchema* _pNodeSchema;

	//luoluo 8-1
	predWindowString * _predWindow;

}; //class IntermediateNodeString
}  //namespace AutomatonCompiler
}  //namespace Cayuga

#endif //_INTERMEDIATE_NODE_STRING_H
