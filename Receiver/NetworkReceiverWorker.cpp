/**
* @file NetworkReceiverWorker.cpp
* @date April 15, 2007
* @brief This file defines the class NetworkReceiverWorker.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#include "../Receiver/NetworkReceiverWorker.h"
using namespace Cayuga::Receiver;

#include "../ICDSMSCE/SAXHandler/ConfigMgr.h"
using namespace Cayuga::SAXHandler;

#include "../ICDSMSCE/SystemMgr/Options.h"
using namespace Cayuga::SystemMgr;

#include "network/TCPClientSocket.h"
#include "network/MessageChannel.h"

// #include "SystemMgr/DataTypeMgr.h"
// #include "SystemMgr/StreamSchemaMgr.h"
// using namespace Cayuga::SystemMgr;
// 
// #include "Utils/AttrMap.h"
// #include "Utils/split.h"
// using namespace Cayuga::Utils;

#include "../Receiver/Options/FileReceiverOptions.h"
#include "../Receiver/Options/NetworkEROptions.h"
using namespace Cayuga::Receiver;

#include "BasicDataStructure/Event/Event.h"
#include "BasicDataStructure/StreamID.h"
using namespace Cayuga::BasicDataStructure::EventNS;
using Cayuga::BasicDataStructure::StreamID;

#include "../ICDSMSCE/AutomatonCompiler/convertConstToByteArray.h"
#include "../ICDSMSCE/BasicDataStructure/Window/WindowThread.h"


#include "../ICDSMSCE/SystemMgr/DataTypeMgr.h"
#include "../ICDSMSCE/SystemMgr/StreamSchemaMgr.h"
using namespace Cayuga::SystemMgr;

#include <cassert>
using namespace std;

const string NetworkReceiverWorker ::exp_time="exp_time";
unsigned NetworkReceiverWorker ::count=0;
Timer NetworkReceiverWorker ::timer;
long  long NetworkReceiverWorker ::tim=0;
/*bool NetworkReceiverWorker::_indicate = false;*/
const unsigned short SIZE_OF_BUFFER = 10;
bool NetworkReceiverWorker::allover = false;

TCPClientSocket * NetworkReceiverWorker :: socket_exp = NULL;

NetworkReceiverWorker::NetworkReceiverWorker():_debugOut(cout)
{
}

NetworkReceiverWorker::NetworkReceiverWorker(TCPClientSocket* clntSock,
                                             ostream& debugOut)
    : m_full(SIZE_OF_BUFFER - 1), m_empty(0),_clntSock(clntSock), _messageChannel(NULL), _debugOut(debugOut){
    socket_exp = clntSock;

    //luoluo 8-26
    vector<string> new_vector;
    _ReceiveToStore.push_back(new_vector);

    //luoluo 8-24
    //initialize  the handle
    //											 _mutex = CreateMutex(NULL ,FALSE,NULL);
    //											 _fullsemaphore = CreateSemaphore(NULL,SIZE_OF_BUFFER-1,SIZE_OF_BUFFER,NULL);
    //											 _emptysemaphore = CreateSemaphore(NULL,0,SIZE_OF_BUFFER,NULL);
    //                                              m_full = Semaphore(SIZE_OF_BUFFER - 1);
    //                                              m_empty = Semaphore(0);
}

NetworkReceiverWorker::~NetworkReceiverWorker() {
    cerr<<"network receiver worker destructed."<<endl;
    delete _messageChannel;
    delete _clntSock;
}

//luoluo 3.12
long long  NetworkReceiverWorker :: return_time(){
    /*	 long long tim=timer.returnTimerInMilliSeconds(exp_time);*/
    return tim;
}
unsigned NetworkReceiverWorker :: return_event_number(){
    return count;
}

void NetworkReceiverWorker::open() {
    try {
        _messageChannel = new MessageChannel(_clntSock);

        Message recvMsg;
        unsigned cnt = 0;

        unsigned printFrequency = ConfigMgr::getInstance()->getOptions()->
                _printFrequency;

        //luluo 8-13
        //start the thread.
        WindowThread *pthread = new WindowThread(this);
        pthread->start();

        //luoluo 6-2
        float wsize = inputWindow::getInstance()->getWindowSize();
        recvMsg.setWsize(wsize);
        _messageChannel->send(&recvMsg);
        recvMsg.resetMessage();

        int dex=0;

        while (true) {
            //luoluo 8-27
            if (allover)
            {
//                Sleep(200);
                continue;
            }

            //luoluo 3.12
            timer.addAndStartTimer(exp_time);

            //luoluo 8-25
//            WaitForSingleObject(_fullsemaphore,INFINITE);
//            WaitForSingleObject(_mutex,INFINITE);
            m_full.wait();
            Lock lock = m_mutex.scopedLock();
            while(true)
            {
                _messageChannel->receive(&recvMsg);
                string s="rest";
                if (strcmp(recvMsg.getData(),s.c_str())==0)
                {

                    vector<string> new_vector;
                    _ReceiveToStore.push_back(new_vector);
                    // 				//print the information
                    // 				inputWindow* fortest = _ReceiveToStore.at(dex).first;
                    // 				multiset<Event*, endTimeLess>::iterator  fortestWin = fortest->_windowevents.begin();
                    // 				for (;fortestWin !=fortest->_windowevents.end();++fortestWin)
                    // 				{
                    // 					cout<<"***"<<(*fortestWin)->getStartTime()<<"***"<<(*fortestWin)->getEndTime()<<endl;
                    // 				}
                    cout<<"====="<<_ReceiveToStore.at(dex).size()<<endl;
                    ++dex;
//                    Sleep(200);
                    /*				continue;*/
                    break;
                }

                string test = "over";
                if (strcmp(recvMsg.getData(),test.c_str())==0)
                {
                    NetworkReceiverWorker::allover = true;
                    break;
                }

                //luoluo3.13
                timer.stopTimer(exp_time);
                tim+=timer.returnTimerInMilliSeconds(exp_time);

                if (cnt % printFrequency == 0) {
                    _debugOut
                            <<"["<<_name()<<"] message "<<cnt
                           <<" received"<<endl;
                }
                // luoluo 3.12  former  statement  :  ++cnt;
                count=++cnt;
                _processMessage(recvMsg, dex);

                // 			//for test.
                // 			cout<<recvMsg.getData()<<"---------"<<endl;

                recvMsg.resetMessage();
            }
//            Sleep(100);
//            ReleaseMutex(_mutex);
//            ReleaseSemaphore(_emptysemaphore,1,NULL);
            m_empty.signal();
        }
    }
    catch (cudb::network::exceptions::SocketException&) {
        // 		cerr<<"machine "<<ip<<":"<<port
        // 			<<" not found, retrying"<<endl;
        // 		Thread::sleep (100);
        //goto RETRY;

        //////////////////////////////////////////////////////////////////////////
        // * to get the ip address
        // 		BYTE   *p;
        // 		char   temp[100];
        // 		struct   hostent   *hp;
        // 		char   ip[16];
        //
        // 		if(gethostname(temp,   sizeof(temp))==   0)
        // 		{
        // 			if((hp   =gethostbyname(temp))!=0)
        // 			{
        // 				p   =(BYTE   *)hp-> h_addr;
        // 				printf(ip,   "%d.%d.%d.%d ",   p[0],   p[1],   p[2],   p[3]);
        // /*				MessageBox(NULL,   ip,   "IP   Address ",   MB_OK); */
        // 				cout<<ip<<endl;
        // 			}
        // 		}
        /*printf("the error is : %d \n" ,(cerr=WSAGetLastError()));*/
        // Changed by Michael
//        cout<<"-------"<<WSAGetLastError()<<"--------"<<endl;
        cout << "in NetworkReceiverWorker::open" << endl;
        cerr<<"["<<_name()<<"] connection broken"<<endl;
    }
}

string NetworkReceiverWorker::_name() {
    return "network receiver";
}

void NetworkReceiverWorker::_processMessage(Message& recvMsg , int location) 
{
    string  str(recvMsg.getData());
    /*	 vector<string> insertvector = _ReceiveToStore.at(location);*/
    _ReceiveToStore.at(location).push_back(str);
}
//  void NetworkReceiverWorker ::_processMessage(Message& recvMsg, int location){
//  // 	//luoluo 8-17
//  // 	while(true)
//  // 	{	
//  // 		if (_indicate == true)
//  // 		{
//  /*			_indicate =false;*/
//  /*			Lock mutexlock = _mutexlock.scopedLock();*/
//  			char _delim	 = ConfigMgr::getInstance()->getOptions()->_attrDelimiter;
//  			bool _bStrict = ConfigMgr::getInstance()->getOptions()->_bStrict;
//  			ParseEvent(recvMsg.getData(), _delim, _bStrict, location);
//  // 			_indicate =true;
//  // 			break;
//  /*		}*/
//  /*	}*/
//  }

TCPClientSocket * NetworkReceiverWorker::getTCPClientSocket()
{
    return socket_exp;
}

// void NetworkReceiverWorker::ParseEvent(const string& line, char charDelim, bool bStrict,int n) {
// 	//luoluo 8-13
// /*	Lock mutexlock = _mutexlock.scopedLock();*/
// 	//lock the queue when receiving.
// 	/*_mutexlock.lock();*/
// /*	_mutexlock.setlock();*/
// /*	mutex = false;*/
// 
// 	vector<string> fields;
// 	split(line, fields, charDelim);
// 	//the last three fields are respectively stream name, start and
// 	//end timestamps
// 	Event* ev;
// 	try {
// 		if (fields.size() < 3) {
// 			//cerr<<"The stream file is ill-formed."<<endl;    
// 			throw string("cannot parse event "+line);
// 		}
// 
// 		//now consult inputStreamName to see how to convert fields
// 		StreamID streamID = StreamSchemaMgr::getInstance()->
// 			getStreamID(fields[fields.size()-3]);
// 		const StreamSchema* pSchema = StreamSchemaMgr::getInstance()->
// 			getStreamSchema(streamID);
// 
// 		const size_t szSchema = pSchema->getInternalSchemaSize();
// 		ev = Event::allocEvent(szSchema);
// 
// 		ev->setStartTime(atoi(fields[fields.size()-2].c_str()));
// 		ev->setEndTime(atoi(fields[fields.size()-1].c_str()));
// 		ev->setStreamID(streamID);
// 
// 		char* evArray = ev->getBody();
// 		size_t offset = 0;
// 		size_t numAttributes = pSchema->getNumAttributes();
// 
// 		if (!(numAttributes+3 == fields.size()))cout<<")()()()()()()("<<"the receive size are not equal!"<<endl;
// 		
// 		
// 		//the last two attributes are start and end timestamps
// 		assert (numAttributes+3 == fields.size());
// 		for (AttributeID attrID = 0; attrID < numAttributes; ++attrID) {
// 			size_t attrLen = pSchema->getInternalAttributeSize(attrID);
// 
// 			AttrTypeID attrType = pSchema->getAttributeType(attrID);
// 			string attrTypeName 
// 				= DataTypeMgr::getInstance()->getName(attrType);
// 			Cayuga::AutomatonCompiler::convertConstToByteArray(
// 				attrTypeName,
// 				fields[attrID],
// 				attrLen,
// 				evArray+offset,
// 				false,	//do not ping string constants from events
// 				true	//put in external space
// 				); 
// 			offset += attrLen;
// 		}
// 	}
// 	catch (const string& s) {
// 		//cerr<<"event from stream "<<fields[fields.size()-3]
// 		//	<<" cannot be internalized."<<endl;
// 		cerr<<s<<endl;
// 		if (bStrict) {
// 			throw s;
// 		}
// 		else return;
// 	}
// 
// 	inputWindow * inputEV = _ReceiveToStore.at(n);
// 	inputEV->_windowevents.insert(ev);
// 
// }

//void NetworkReceiverWorker::setNetworkReceiverWorker(TCPClientSocket* clntSock,
//							  ostream& debugOut )
//{
//	_clntSock=clntSock;
//	_messageChannel=NULL;
///*	_debugOut=debugOut;*/
//	socket_exp = clntSock;
//}
//
//void NetworkReceiverWorker::operator = (NetworkReceiverWorker& rhs)
//{
//	rhs._clntSock = _clntSock;
//	rhs._messageChannel = _messageChannel;
//	rhs.socket_exp=socket_exp;
//	rhs._mutexlock=_mutexlock;
//	rhs._ReceiveToStore=_ReceiveToStore;
//}

//void threadproc(void * param)
//{
//	NetworkReceiverWorker * pthis = (NetworkReceiverWorker*)param;
//	inputWindow *	pWindowThread = inputWindow::getInstance();
//	while(true)
//	{
//	WaitForSingleObject(pthis->_emptysemaphore,INFINITE);
//	WaitForSingleObject(pthis->_mutex,INFINITE);
//	vector<pair<inputWindow* , bool >>* expr  = 
//			pthis->GetInputwinVector();
//
//		if ((*expr).empty())continue;
//		vector<pair<inputWindow* , bool >>::iterator expr1 = (*expr).begin();
//		for (;expr1 != (*expr).end();++expr1)
//		{
//			if(!pWindowThread->slide_over)break;
//			if ((*expr1).second == TRUE)
//			{	
//
//				inputWindow * ptr = (*expr1).first;
//
//				// 						inputWindow* fortest1= pWindowThread;
//				// 						multiset<Event*, endTimeLess>::iterator  fortestWin1 = fortest1->_windowevents.begin();
//				// 						for (;fortestWin1 !=fortest1->_windowevents.end();++fortestWin1) 
//				// 						{
//				// 							cout<<"###"<<(*fortestWin1)->getStartTime()<<"###"<<(*fortestWin1)->getEndTime()<<endl;
//				// 						}
//
//				//
//				pWindowThread->copyTwoWinEv(ptr);
//				//
//				inputWindow* fortest= pWindowThread;
//				multiset<Event*, endTimeLess>::iterator  fortestWin = fortest->_windowevents.begin();
//				for (;fortestWin!=fortest->_windowevents.end();++fortestWin) 
//				{
//					cout<<"$$$"<<(*fortestWin)->getStartTime()<<"$$$"<<(*fortestWin)->getEndTime()<<endl;
//				}
//
//				pWindowThread->slide_over = false;
//				(*expr1).second = FALSE;
//				break;
//			}
//		}
//
//		//to indicate that the processing is over.
//		if (expr1 == (*expr).end() && !pWindowThread->_windowevents.empty()&&(*((*expr).rbegin())).second ==FALSE)pWindowThread->Allover =true;
//		Sleep(100);
//		ReleaseMutex(pthis->_mutex);
//		ReleaseSemaphore(pthis->_fullsemaphore,1,NULL);
//	}
//}
//
//void NetworkReceiverWorker::testthread(void)
//{
//	_beginthread(threadproc,0,this);
//}
