#include "ioloopthread.h"

#include <tr1/functional>

#include "ioloop.h"
#include "threadutil.h"


namespace tpush
{

    IOLoopThread::IOLoopThread()
    {
        m_ioloop = new IOLoop();
        std::tr1::function<void ()> func = std::tr1::bind(&IOLoop::start, 
                                                        m_ioloop);
        
        m_thread = new Thread(func);
    }

    IOLoopThread::~IOLoopThread()
    {
        delete m_thread;
        delete m_ioloop;
    }
    
    void IOLoopThread::start()
    {
        m_thread->start();
    }

    void IOLoopThread::stop()
    {
        m_ioloop->stop();
        m_thread->stop();    
    }
}
