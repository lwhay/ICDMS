/**
* @file NetworkReceiverWorker.h
* @date April 15, 2007
* @brief This file declares the class NetworkReceiverWorker.
* @author Mingsheng Hong (mshong@cs.cornell.edu)
*/

#ifndef _NETORK_RECEIVER_WORKER_H
#define _NETORK_RECEIVER_WORKER_H

#ifdef WIN32
#include <process.h>
#endif
#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

namespace cudb {
	namespace network {
		class TCPClientSocket;
		class MessageChannel;
		class Message;
	}
}
using namespace cudb::network;


#include "receiver.h"
#include "../ICDSMSCE/Utils/Timer.h"
using namespace Cayuga::Utils;
#include "ER/RelER.h"
#include "../ICDSMSCE/Utils/Singleton.h"
using  Cayuga::Utils::Singleton;

#include "corelib/os/threads/Mutex.h"
#include "corelib/os/threads/Semaphore.h"
using namespace cudb::corelib::os::threads;


#include "../ICDSMSCE/BasicDataStructure/Window/inputWindow.h"
/*#include "BasicDataStructure/Window/WindowThread.h"*/
	class WindowThread;
namespace Cayuga {
namespace Receiver {
		/**
		* @brief The Network Receiver Worker. 
		* @author Mingsheng Hong (mshong@cs.cornell.edu)
		* @date April 15, 2007
		* @version 1.0
		*/
		class NetworkReceiverWorker :
			public Singleton<NetworkReceiverWorker>{
		public:
			friend class WindowThread;
			/**
			* @brief Default constructor
			* @param debugOut The debug message output stream
			* @author Mingsheng Hong (mshong@cs.cornell.edu)
			* @date April 15, 2007
			* @version 1.0
			*/
			NetworkReceiverWorker();

			NetworkReceiverWorker(TCPClientSocket* clntSock,
				ostream& debugOut = cout);

			/**
			* @brief Virtual destructor
			* @author Mingsheng Hong (mshong@cs.cornell.edu)
			* @date April 15, 2007
			* @version 1.0
			*/
			virtual ~NetworkReceiverWorker();

			/**
			* @brief Start receiving inputs.
			* @author Mingsheng Hong (mshong@cs.cornell.edu)
			* @date April 15, 2007
			* @version 1.0
			*/
			virtual void open();

			/**
			* @brief Return a string label for the name of this receiver instance.
			* @return As above.
			* @author Mingsheng Hong (mshong@cs.cornell.edu)
			* @date April 15, 2007
			* @version 1.0
			*/
			static string name();

			//luoluo 3.12
			static	long long return_time();
			static	 unsigned return_event_number();
		
			//luoluo 6-17
			/**
			*@brief		Return the clientsocket to the main function ,then the socket
							could be used in the other filed (though maybe there have some
							danger.)
			*/
			TCPClientSocket *  getTCPClientSocket ();
			static TCPClientSocket * socket_exp;
			
			//luoluo 8-15
			//This variable is used to control access to the window queue.	 
			Mutex _mutexlock;

			//luoluo 8-17
			static bool _indicate;

			//luoluo 8-24
			//the following three handle will be used for testing.
//			HANDLE _mutex;
//			HANDLE _fullsemaphore;
//			HANDLE _emptysemaphore;
            Mutex m_mutex;
            Semaphore m_full;
            Semaphore m_empty;

		protected:
			//change by luoluo 6-18
			/*virtual void _processMessage(Message& recvMsg) = 0;*/
			virtual void _processMessage(Message& recvMsg , int location) ;
			/**
			* @brief Return a string label for the name of this receiver instance.
			* @return As above.
			* @author Mingsheng Hong (mshong@cs.cornell.edu)
			* @date April 15, 2007
			* @version 1.0
			*/
			//change by luoluo 6-18
			/*virtual string _name() = 0;*/
			virtual string _name() ;

        public:
			/**
			*@brief		be used as a cache.
			*@param	_soreWin   to receive the message and sore it .
							index         to pridicate whether the information
											 are transformed in the only inputWindow.
			 *@author	luoluo 8-13
            */
/*			vector<pair<inputWindow* , bool >> _ReceiveToStore;*/
/*			vector<inputWindow* > _ReceiveToStore;*/
            vector< vector<string> > _ReceiveToStore;

			//luoluo 8-27
			//to indicate that the receive work is down.
			static bool allover ;

		private:
			/** 
			* This variable stores the socket connection that we will maintain. 
			*/
			 TCPClientSocket* _clntSock;

			/**
			* This variable stores the corresponding message channel for the
			* above socket connection. 
			* If we want to removed redundant buffer copy in reading socket buffer, 
			* MessageChannel should to be replaced by lower level functions that read 
			* socket buffer directly.
			*/
			MessageChannel* _messageChannel;

			//The debug message output stream 
			ostream& _debugOut;

			//luoluo 3.12 
			// the two variables  are used for record the time for receiving events.
		static	Timer timer;
		static	const string exp_time ;
       static   unsigned  count;
	   static long long tim;

		}; //class NetworkReceiverWorker
	}  //namespace Receiver
}  //namespace Cayuga

#endif //_NETORK_RECEIVER_WORKER_H
