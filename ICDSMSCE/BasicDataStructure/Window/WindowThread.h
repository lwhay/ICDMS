/**
 * @file WindowThread.h
 * @brief This file declares the class WindowThread.
 * @author leixiaoluo
 */

#ifndef _WINDOW_THREAD_H
#define _WINDOW_THREAD_H

#include <iostream>
#include <vector>
using namespace std;

#include "../Receiver/NetworkReceiverWorker.h"
using  namespace Cayuga::Receiver;

#include "corelib/os/threads/Thread.h"
using cudb::corelib::os::threads::Thread;

#include "corelib/os/threads/Mutex.h"
//#include "corelib/os/threads/Semaphore.h"
using namespace cudb::corelib::os::threads;
/*#include "network/Mutex.h"*/

#include "BasicDataStructure/Window/inputWindow.h"

class WindowThread : public Thread {
public:
    WindowThread(NetworkReceiverWorker * ptrTHIS)
    {
        pNetworkReceiverWorker = ptrTHIS;
    }
    ~WindowThread()
    {
        delete pNetworkReceiverWorker;
    }
protected:
    void* run ()
    {
        try
        {
            while(true)
            {
                //				Sleep(200);
                if(!pNetworkReceiverWorker->allover)
                {
                    // Changed by Michael;
                    //	  			WaitForSingleObject(pNetworkReceiverWorker->_emptysemaphore,INFINITE);
                    //	  			WaitForSingleObject(pNetworkReceiverWorker->_mutex,INFINITE);
                    pNetworkReceiverWorker->m_empty.wait();
                    Lock lock = pNetworkReceiverWorker->m_mutex.scopedLock();

                    //to probe whether could input the copy to the inputQueue.
                    if (inputWindow::getInstance()->slide_over)
                    {
                        //to avoid the influence created by this parameter slide_over.
                        inputWindow::getInstance()->slide_over = false;

                        //copy the indexed line in networkreceiverworker.
                        int pos = inputWindow::getInstance()->_indicateLine;
                        /*	pWindowBuffer=pNetworkReceiverWorker->_ReceiveToStore.at(pos);*/
                        tempro = pNetworkReceiverWorker->_ReceiveToStore.at(pos);

                        inputWindow::getInstance()->copyTwoWinEv(tempro);

                    }


                    // 				//for test
                    // 				cout<<"++++"<<pNetworkReceiverWorker->_ReceiveToStore.size()<<"+++"
                    // 								<<pos<<endl;

                    // Changeed by Michael
                    //	 			Sleep(100);
                    //ReleaseMutex(pNetworkReceiverWorker->_mutex);
                    //ReleaseSemaphore(pNetworkReceiverWorker->_fullsemaphore,1,NULL);
                    pNetworkReceiverWorker->m_full.signal();
                }
                else
                {

                    //to probe whether could input the copy to the inputQueue.
                    if (inputWindow::getInstance()->slide_over)
                    {
                        //to avoid the influence created by this parameter slide_over.
                        inputWindow::getInstance()->slide_over = false;

                        int pos = inputWindow::getInstance()->_indicateLine;
                        if(pNetworkReceiverWorker->_ReceiveToStore.size() == pos+1)
                        {
                            inputWindow::getInstance()->Allover = true;
                            break;
                        }
                        else
                            tempro = pNetworkReceiverWorker->_ReceiveToStore.at(pos);

                        inputWindow::getInstance()->copyTwoWinEv(tempro);
                    }
                    // 					int pos = inputWindow::getInstance()->_indicateLine;
                    //
                    // 				   //to indicate that all the work of put event into inputqueue is over.
                    // // 					if (pNetworkReceiverWorker->_ReceiveToStore.begin()+pos+1
                    // // 								== pNetworkReceiverWorker->_ReceiveToStore.end())
                    // 					if(pNetworkReceiverWorker->_ReceiveToStore.size() == pos+1)
                    // 					{
                    // 						inputWindow::getInstance()->Allover = true;
                    // 						break;
                    // 					}
                    // 					else
                    // 						tempro = pNetworkReceiverWorker->_ReceiveToStore.at(pos);
                }

                // 				//to probe whether could input the copy to the inputQueue.
                // 				if (inputWindow::getInstance()->slide_over)
                // 				{
                // 					inputWindow::getInstance()->copyTwoWinEv(tempro);
                // 					inputWindow::getInstance()->slide_over = false;
                // 				}
            }
        }
        catch (exception e)
        {
            cerr << "-------" << "exception: " << e.what() << "--------" << endl;
            //            cerr <<GetLastError()<<endl;
            cerr << "WindowThread error" << endl;
        }
        return NULL;
    }
private:
    NetworkReceiverWorker* pNetworkReceiverWorker; vector<string>tempro;
}; //class WindowThread
#endif //_WINDOW_THREAD_H
