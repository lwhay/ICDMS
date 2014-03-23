    /**
     * @file Options.h
     * @date October 20, 2006
     * @brief An Options object stores all system configuration options.
     * @author Mingsheng Hong (mshong@cs.cornell.edu)
     */
    
    #ifndef _OPTIONS_H
    #define _OPTIONS_H
    
    
    #include "SAXHandler/DataSource.h"
    using Cayuga::SAXHandler::DataSource;
    
    #include "Utils/StringMap.h"
    using Cayuga::Utils::StringMap;
    
    namespace Cayuga {
    
    namespace SystemMgr {
    /**
     * @brief An Options object stores all system configuration options.
     * @author Mingsheng Hong (mshong@cs.cornell.edu)
     * @date December 8, 2005
     * @version 1.0
     */
    class Options {
    public:
    	/**
    	* @brief Set the option values from the intput option name, value pairs
    	* @author Mingsheng Hong (mshong@cs.cornell.edu)
    	* @date December 8, 2005
    	* @version 1.0
    	*/
    	void setOptions(const StringMap& optionMap);
    
    	//Query
    	/**
    	* This variable indicates whether the internal state of the system will be 
    	* dumped/check-pointed into disk.
    	*/
    	///@todo Implement the check point procedure.
    	bool _saveInternalState;
    	
    	/**
    	* This variable indicates HOW to read queries. Currently queries can be read
    	* from a single file (FILE), or from all files of a given directory (DIR).
    	* @invariant If the query input mode is DIR, we require that the names of
    	* all the XML files within that directory are prefixed with "AIR_", followed
    	* by the ID of that query, starting from 0.
    	*/
    	DataSource _queryInputMode;
    	
    	/**
    	* This variable indicates WHERE to read queries. If the query input mode is
    	* FILE, the file name is stored here. If the mode is DIR, the directory name
    	* is stored here.
    	*/
    	string _queryInputName;
    	
    	/**
    	* This variable indicates the number of queries to read. In DIR mode,
    	* the files of name from "AIR_0.xml" to 
    	* "AIR_n.xml" where n is _queryNumber-1 will be read. By defeault it is 0.
    	*/
    	unsigned _queryNumber;
    
    	/**
    	* This variable indicates whether incoming queries are in the form of AIR
    	* or high-level CayugaQL expression.
    	*/
    	bool _bAirQueries;
    
    	//Doc
    	/**
    	* This variable indicates stores the locations of files that encode input
    	* stream schemas.
    	*/
    	vector<string> _streamSchemaFiles;
    	
    	/**
    	* This variable indicates HOW to read documents. Currently documents can be 
    	* read from a single file (FILE), or from all files of a given directory 
    	* (DIR).
    	* @invariant If the doc input mode is DIR, we require that the names of
    	* all the XML files within that directory are prefixed with "doc_", followed
    	* by the ID of that document, starting from 0.
    	*/
    	DataSource _docInputMode;
    	
    	/**
    	* This variable indicates WHERE to read documents. If the doc input mode is
    	* FILE, the file name is stored here. If the mode is DIR, the directory name
    	* is stored here.
    	*/
    	string _docInputName;
    	
    	/**
    	* This variable indicates the name of the input stream. If the name is
    	* XML, the stream is XML. If the name is something else such as Stock, 
    	* the stream is relational.
    	*
    	* This variable is obsolete! 
    	*/
    	//string _docInputStream;
    
    	/**
    	* This variable indicates whether the input stream is XML. 
    	* If so it invokes a special event parser called CaxParser instead of the
    	* regular relational event parser. By default it is false.
    	*/
    	bool _bXMLStream;
    
    	/**
    	* This variable indicates the number of documents to read. This is only used
    	* if the doc input mode is DIR. So files of name from "doc_0.xml" to 
    	* "doc_n.xml" where n is _docNumber-1 will be read.
    	*/
    	unsigned _docNumber;
    
    	/**
    	* This variable indicates whether the inputs events will be received from
    	* TCP sockets. If so, it stores the TCP socket port the network receiver
    	* server uses. Otherwise it is 0. By default it is 0.
    	*/
    	short _nERPort;
    
    	/**
  	* This variable lets the user set the NIC interface to connect to when
  	* in NETWORK mode and Cayuga is to use a server socket to receive data.
  	* By default it is "", where Cayuga will let the OS decide.
  	*/
  	string _LocalNICInterface;
  
  	/**
    	* This variable stores the character delimiter for attributes for FileER.
    	*/
    	char _attrDelimiter;
    
    	//GC
    	/**
    	* This variable stores the size of the memory pool used by garbage 
    	* collector.
    	*/
    	unsigned _gcSize;
    	
    	/**
    	* This variable stores the frequency of invoking garbage collector, defined
    	* to be the number of events processed between every two GC calls.
    	*
    	* This variable is obsolete! GC frequency cannot be controlled by config.
    	*/
    	//unsigned _gcFrequency;
    
    	/**
    	* This variable stores the unit of GC-managed memory pool size. 
    	*
    	* Its value could be drawn from {BYTE, KB, MB}. MB is the default value.
    	*/
    	string _gcSizeUnit;
    
    	//debugging, measuring, logging
    	/**
    	* This variable indicates whether information of internal system state 
    	* should be output to screen. By default it is false.
    	*/
    	bool _bVerbose;
    
    	/**
    	* This variable stores the frequency of printing debug messages. 
    	* By default it is 1000.
    	*/
    	unsigned _printFrequency;
    
    	/**
    	* This variable stores the destination of debug messages.
    	* If it is "", debug messages will be printed to screen.
    	* Otherwise, the value of this variable should be the composition of the
    	* name of a path relative to the Cayuga home directory, 
    	* and the name of a file which stores the debug messages. 
    	* An example would be "log/debugMsg.txt". Please use / instead
    	* of other characters such as \ to specify the path name separator.
        * By default it is "".
    	*/
    	string _debugMessageDestination;
    
    	/**
    	* This variable indicates whether the output events will be logged
        * into a disk file named witnesses.txt. If it is "",
        * witnesses will not be logged. Otherwise, it specifies the name of
        * a directory relative to the top-level directory
        * in which the witness file will be stored.
        */
    	string _witnessLogDir;
    
    	/**
    	* This variable is true iff _witnessLogDir is not "".
        */
    	bool _bLogOutput;
    	
    	/**
    	* This variable indicates whether information of internal system state 
    	* should be recorded to disk file.
    	*/
    	bool _bRecordTrace;
    	
    	/**
    	* This variable indicates whether information of internal system state 
    	* should be sent across the network. If so, it stores the TCP socket 
    	* port the trace manager uses to talk to the visualizer. Otherwise it is 0.
    	*/
    	short _nTracePort;
    	
    	/**
    	* This variable indicates whether the output events will be sent out to
    	* a TCP socket. If so, it stores the TCP socket port the network output 
    	* event sender uses. Otherwise it is 0. By default it is 0.
    	*/
    	short _nOutputEventPort;
    
  	/** If _nOutputEventPort is not 0, this variable sets the hostname where
  	* Cayuga will try to connect to send witness (output) events. By default
  	* it is "".
  	*/
  	string _OutputEventHost;
  
    	/**
    	* This variable stores the name of directory in which the compiled AIR 
    	* queries will be stored. By default it is "", in which case the 
    	* compiled AIR queries will not be logged.
    	*/
    	string _compiledAIRLogDir;
    
    	/**
    	* This variable stores the name of directory in which the commands
        * that are received at run-time will be stored. By default it is "",
        * in which case the commands will not be logged.
    	*/
    	string _commandLogDir;
    
    	/**
    	* This variable indicates whether the Cayuga measure manager should be 
    	* turned on.
    	*/
    	bool _bMeasure;
    
    	/**
    	* This variable indicates whether information of cayuga measures
    	* should be sent across the network. If so, it stores the TCP socket 
    	* port the measure manager uses. Otherwise it is 0.
    	*/
    	short _nMeasurePort;
    
    	/**
    	* This variable stores the length of the event window with which the event
    	* processing costs are aggregated. It is measured by the number of 
    	* consecutive input events Cayuga processes.
    	*/
    	unsigned _eventWindowLen;
    
    	/**
    	* This variable stores the (time) frequency of checkpointing the system.
    	* If the value is n, it means for every period of n timeunits, the entire
    	* state of the system will be checkpointed. A special value 0 means no
    	* checkpoints will be taken.
    	* By default it is 0.
    	*/
    	long _checkPointFrequency;
    
    	/**
    	* This variable stores the name of directory in which the check points
    	* and trace messages will be stored. By default it is ".".
    	*/
    	string _checkPointAndTraceDir;
    		
    	/**
    	* This variable indicates whether Cayuga should tolerate any run-time 
    	* errors, such as in new query addition. It is true if it does not 
    	* tolerate any errors. By default it is false;
    	*/
    	bool _bStrict;
    
    	//run-time commands
    	/**
    	* This variable indicates whether the Cayuga command server should be 
    	* on. If so, it stores the TCP socket port the command server uses to talk 
    	* to any client that submits commands to the engine at run-time.
    	* Otherwise it is 0.
    	*/
    	short _nCommandPort;
    
    	//CEL compiler
    	/**
    	* This variable indicates whether the CEL compiler should try to inline
    	* the generated AIR queries if possible. For example, if the second input
    	* expression of a binary operator is a unary expression, then instead of 
    	* generating a separate automaton for it, we could inline it with the 
    	* automaton corresponding to the binary operator, by pulling the 
    	* predicates in the unary expression to the filter edge of that unary
    	* operator.
    	* By default it is true.
    	*/
    	bool _bInlineQuery;
    
    	/**
    	* This variable indicates whether automaton states should be merged.
    	* By default it is true.
    	*/
    	bool _bMergeStates;
    
    	/**
    	* @brief Normalize the input directory name as follows. 
    			If it is non-empty, and does not end with /, then add / to the end.
        * @param dirName The input directory name to normalize.
    	* @author Mingsheng Hong (mshong@cs.cornell.edu)
    	* @date April 9, 2007
    	* @version 1.0
    	*/
    	
    	static void normalizeDirectory(string& dirName);
    
    private:
    	/**
    	* @brief Set the option value from the input option name, value pairs of 
    			 string format.
        * @param optionMap The map that contains all option name, value pairs of
    			 string format read from XML.
    	* @param memberVar The option variable to set
    	* @param optionName The string name of the option
    	* @param defaultValue The default value of the option, if the value is not
    			 overridden by the user in XML.
    	* @author Mingsheng Hong (mshong@cs.cornell.edu)
    	* @date December 8, 2005
    	* @version 1.0
    	*/
    	template <typename T>
    	void _setOption(const StringMap& optionMap,
    		T& memberVar, 
    		const string& optionName,
    		const T& defaultValue);
    
    }; //class Options
    }  //namespace SAXHandler
    }  //namespace Cayuga
    
 #endif //_OPTIONS_H
