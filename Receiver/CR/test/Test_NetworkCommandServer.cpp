/**
 * @file Test_NetworkCommandServerThread.cpp
 * @date March 23, 2007
 * @brief This file tests the class NetworkCommandServerThread.
 * @author Mingsheng Hong (mshong@cs.cornell.edu)
 */

#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#include "extlib/inc/cudb/network/TCPClientSocket.h"
#include "extlib/inc/cudb/network/MessageChannel.h"
using namespace cudb::network;

#include "extlib/inc/cudb/corelib/os/threads/thread.h"
using namespace cudb::corelib::os::threads;

//Builder pattern: do connect, buildQueryCommand, sendCommand in a sequence
class CommandClient {
public:
	CommandClient() : _clntSock(NULL), _messageChannel(NULL), _pMsg(NULL) {
	}

	void connect() {
		try {
			_clntSock = new TCPClientSocket("127.0.0.1", 6000);
			_messageChannel = new MessageChannel(_clntSock);
			cout<<"new CR client connection "<<"established."<<endl;
		}
		catch (cudb::network::exceptions::SocketException&) {
			//cerr<<"machine "<<ip<<":"<<port
			//	<<" not found, retrying"<<endl;
			cerr<<"command server is not up."<<endl;
		}
	}

	void buildQueryCommand(const string& queryFileName) {
		delete _pMsg;

		string data = "3`QUERY`AIR`";
		ifstream inAIRFile(queryFileName.c_str());
		//cout<<inAIRFile.rdbuf();
		
		ostringstream oStr;
		oStr<<inAIRFile.rdbuf();
		//cout<<oStr;
		data += oStr.str();
		data += "`";
		cout<<data<<endl;
		_pMsg = new Message(data.c_str(), data.size());		
	}

	void buildSchemaCommand(const string& schemaFileName) {
		delete _pMsg;

		string data = "2`SCHEMA`";
		ifstream inAIRFile(schemaFileName.c_str());
		//cout<<inAIRFile.rdbuf();
		
		ostringstream oStr;
		oStr<<inAIRFile.rdbuf();
		//cout<<oStr;
		data += oStr.str();
		data += "`";
		cout<<data<<endl;
		_pMsg = new Message(data.c_str(), data.size());		
	}


	void sendCommand() {
		try {
			_messageChannel->send(_pMsg);
		}
		catch (cudb::network::exceptions::SocketException&) {
			cerr<<"cannot send command."<<endl;
		}
	}

private:
	TCPClientSocket* _clntSock;
	MessageChannel* _messageChannel;
	Message* _pMsg;
};

//argv[1] is the input query file name
void main(int argc, char** argv) {
	if (argc != 2) return;

	CommandClient cc;

	cc.connect();
	cc.buildQueryCommand(argv[1]);
	cc.sendCommand();

	cc.buildSchemaCommand("Cayuga/demo/webserver/webfeeds_schema.xml");
	cc.sendCommand();

	while (true)
		Thread::sleep(1000);
}
