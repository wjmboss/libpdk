// @copyright 2017-2018 zzu_softboy <zzu_softboy@163.com>
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
// OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
// NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
// THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Created by softboy on 2017/03/13.

#include "gtest/gtest.h"

#ifdef PDK_OS_UNIX
#include <pthread.h>
#endif
#if defined(PDK_OS_WIN)
#include <windows.h>
#if defined(PDK_OS_WIN32)
#include <process.h>
#endif
#endif
#include "pdk/kernel/CallableInvoker.h"
#include "pdk/base/os/thread/internal/ThreadPrivate.h"

#include "pdk/kernel/CoreApplication.h"
#include "TestEventLoop.h"
#include "pdk/kernel/Timer.h"
#include "pdk/kernel/ElapsedTimer.h"
#include "pdk/base/time/Time.h"
#include "pdk/kernel/Pointer.h"
#include "pdk/base/lang/String.h"
#include <mutex>
#include <tuple>
#include <condition_variable>

#ifndef PDK_NO_EXCEPTIONS
#include <exception>
#endif
#include "pdk/base/os/thread/Thread.h"
#include "pdk/base/os/thread/Semaphore.h"
#include "pdk/stdext/typetraits/Sequence.h"
#include "pdk/stdext/typetraits/CallableInfoTrait.h"

enum { one_minute = 60 * 1000, five_minutes = 5 * one_minute };

using pdk::os::thread::Thread;
using pdk::kernel::EventLoop;
using pdk::kernel::Object;
using pdk::kernel::Timer;
using pdk::kernel::Pointer;
using pdk::kernel::CoreApplication;
using pdk::kernel::CallableInvoker;
using pdk::time::Time;
using pdkunittest::TestEventLoop;
using pdk::os::thread::Semaphore;
using pdk::kernel::ElapsedTimer;
using pdk::lang::String;
using pdk::lang::Latin1String;

class CurrentThread : public Thread
{
public:
   pdk::HANDLE m_id;
   Thread *m_thread;
   
   void run()
   {
      m_id = Thread::getCurrentThreadId();
      m_thread = Thread::getCurrentThread();
   }
};

class SimpleThread : public Thread
{
public:
   std::mutex m_mutex;
   std::condition_variable m_cond;
   
   void run()
   {
      std::lock_guard locker(m_mutex);
      m_cond.notify_one();
   }
};

class ExitObject : public Object
{
public:
   Thread *m_thread;
   int m_code;
   void slot() 
   { 
      m_thread->exit(m_code);
   }
};

class ExitThread : public SimpleThread
{
public:
   ExitObject *m_object;
   int m_code;
   int m_result;
   
   void run()
   {
      SimpleThread::run();
      if (m_object) {
         m_object->m_thread = this;
         m_object->m_code = m_code;
         Timer::singleShot(100, m_object, &ExitObject::slot);
      }
      m_result = exec();
   }
};

class TerminateThread : public SimpleThread
{
public:
   void run()
   {
      setTerminationEnabled(false);
      {
         std::unique_lock locker(m_mutex);
         m_cond.notify_one();
         m_cond.wait_for(locker, std::chrono::milliseconds(five_minutes));
      }
      setTerminationEnabled(true);
      FAIL() << "ThreadTest : test case hung";
   }
};

class QuitObject : public Object
{
public:
   Thread *m_thread;
public:
   void slot()
   {
      m_thread->quit();
   }
};

class QuitThread : public SimpleThread
{
public:
   QuitObject *m_object;
   int m_result;
   
   void run()
   {
      SimpleThread::run();
      if (m_object) {
         m_object->m_thread = this;
         Timer::singleShot(100, [&](){
            m_object->slot();
         });
      }
      m_result = exec();
   }
};

class SleepThread : public SimpleThread
{
public:
   enum SleepType { Second, Millisecond, Microsecond };
   
   SleepType m_sleepType;
   int m_interval;
   int m_elapsed; // result, in *MILLISECONDS*
   void run()
   {
      std::lock_guard<std::mutex> locker(m_mutex);
      m_elapsed = 0;
      Time time;
      time.start();
      switch (m_sleepType) {
      case Second:
         sleep(m_interval);
         break;
      case Millisecond:
         msleep(m_interval);
         break;
      case Microsecond:
         usleep(m_interval);
         break;
      }
      m_elapsed = time.elapsed();
      m_cond.notify_one();
   }
};

//TEST(ThreadTest, testCurrentThreadId)
//{
//   CurrentThread thread;
//   thread.m_id = 0;
//   thread.m_thread = nullptr;
//   thread.start();
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(thread.m_id != 0);
//   ASSERT_TRUE(thread.m_id != Thread::getCurrentThreadId());
//}

//TEST(ThreadTest, testCurrentThread)
//{
//   ASSERT_TRUE(Thread::getCurrentThread() != nullptr);
//   CurrentThread thread;
//   thread.m_id = 0;
//   thread.m_thread = nullptr;
//   thread.start();
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_EQ(thread.m_thread, dynamic_cast<Thread *>(&thread));
//}

//TEST(ThreadTest, testIdealThreadCount)
//{
//   ASSERT_TRUE(Thread::getIdealThreadCount() > 0);
//   std::clog << "Ideal thread count:" << Thread::getIdealThreadCount();
//}

//TEST(ThreadTest, testIsFinished)
//{
//   SimpleThread thread;
//   ASSERT_TRUE(!thread.isFinished());
//   std::unique_lock<std::mutex> locker(thread.m_mutex);
//   thread.start();
//   ASSERT_TRUE(!thread.isFinished());
//   thread.m_cond.wait(locker);
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(thread.isFinished());
//}

//TEST(ThreadTest, testIsRunning)
//{
//   SimpleThread thread;
//   ASSERT_TRUE(!thread.isRunning());
//   std::unique_lock<std::mutex> locker;
//   thread.start();
//   ASSERT_TRUE(thread.isRunning());
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(!thread.isRunning());
//}

//TEST(ThreadTest, testSetPriority)
//{
//   SimpleThread thread;
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::IdlePriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::LowestPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::LowPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::NormalPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::HighPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::HighestPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::TimeCriticalPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);

//   std::unique_lock<std::mutex> locker(thread.m_mutex);
//   thread.start();

//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::IdlePriority);
//   ASSERT_EQ(thread.getPriority(), Thread::IdlePriority);
//   thread.setPriority(Thread::LowestPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::LowestPriority);
//   thread.setPriority(Thread::LowPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::LowPriority);
//   thread.setPriority(Thread::NormalPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::NormalPriority);
//   thread.setPriority(Thread::HighPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::HighPriority);
//   thread.setPriority(Thread::HighestPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::HighestPriority);
//   thread.setPriority(Thread::TimeCriticalPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::TimeCriticalPriority);

//   thread.m_cond.wait(locker);
//   ASSERT_TRUE(thread.wait(five_minutes));

//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::IdlePriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::LowestPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::LowPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::NormalPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::HighPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::HighestPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//   thread.setPriority(Thread::TimeCriticalPriority);
//   ASSERT_EQ(thread.getPriority(), Thread::InheritPriority);
//}

//TEST(ThreadTest, testSetStackSize)
//{
//   SimpleThread thread;
//   ASSERT_EQ(thread.getStackSize(), 0u);
//   thread.setStackSize(8192u);
//   ASSERT_EQ(thread.getStackSize(), 8192u);
//   thread.setStackSize(0u);
//   ASSERT_EQ(thread.getStackSize(), 0u);
//}

//TEST(ThreadTest, testExit)
//{
//   ExitThread thread;
//   thread.m_object = new ExitObject;
//   thread.m_object->moveToThread(&thread);
//   thread.m_code = 42;
//   thread.m_result = 0;
//   ASSERT_TRUE(!thread.isFinished());
//   ASSERT_TRUE(!thread.isRunning());
//   std::unique_lock<std::mutex> locker(thread.m_mutex);
//   thread.start();

//   ASSERT_TRUE(thread.isRunning());
//   ASSERT_TRUE(!thread.isFinished());
//   thread.m_cond.wait(locker);
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(thread.isFinished());
//   ASSERT_TRUE(!thread.isRunning());
//   ASSERT_EQ(thread.m_result, thread.m_code);
//   delete thread.m_object;

//   ExitThread thread2;
//   thread2.m_object = nullptr;
//   thread2.m_code = 53;
//   thread2.m_result = 0;
//   std::unique_lock<std::mutex> locker2(thread2.m_mutex);
//   thread2.start();
//   thread2.exit(thread2.m_code);
//   thread2.m_cond.wait(locker2);
//   ASSERT_TRUE(thread2.wait(five_minutes));
//   ASSERT_EQ(thread2.m_result, thread2.m_code);
//}

//TEST(ThreadTest, testStart)
//{
//   Thread::Priority priorities[] = {
//      Thread::Priority::IdlePriority,
//      Thread::Priority::LowestPriority,
//      Thread::Priority::LowPriority,
//      Thread::Priority::NormalPriority,
//      Thread::Priority::HighPriority,
//      Thread::Priority::HighestPriority,
//      Thread::Priority::TimeCriticalPriority,
//      Thread::Priority::InheritPriority
//   };

//   const int priorityCount = sizeof(priorities) / sizeof(Thread::Priority);
//   for (int i = 0; i < priorityCount; ++i) {
//      SimpleThread thread;
//      ASSERT_TRUE(!thread.isFinished());
//      ASSERT_TRUE(!thread.isRunning());
//      std::unique_lock<std::mutex> locker(thread.m_mutex);
//      thread.start(priorities[i]);
//      ASSERT_TRUE(thread.isRunning());
//      ASSERT_TRUE(!thread.isFinished());
//      thread.m_cond.wait(locker);
//      ASSERT_TRUE(thread.wait(five_minutes));
//      ASSERT_TRUE(thread.isFinished());
//      ASSERT_TRUE(!thread.isRunning());
//   }
//}

//TEST(ThreadTest, testTerminate)
//{
//   TerminateThread thread;
//   {
//      std::unique_lock<std::mutex> locker(thread.m_mutex);
//      thread.start();
//      ASSERT_TRUE(std::cv_status::no_timeout == thread.m_cond.wait_for(locker, std::chrono::milliseconds(five_minutes)));
//      thread.terminate();
//      thread.m_cond.notify_one();
//   }
//   ASSERT_TRUE(thread.wait(five_minutes));
//}

//TEST(ThreadTest, testQuit)
//{
//   QuitThread thread;
//   thread.m_object = new QuitObject;
//   thread.m_object->moveToThread(&thread);
//   thread.m_result = -1;
//   ASSERT_TRUE(!thread.isFinished());
//   ASSERT_TRUE(!thread.isRunning());

//   std::unique_lock<std::mutex> locker(thread.m_mutex);
//   thread.start();
//   ASSERT_TRUE(!thread.isFinished());
//   ASSERT_TRUE(thread.isRunning());
//   thread.m_cond.wait(locker);
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(thread.isFinished());
//   ASSERT_TRUE(!thread.isRunning());
//   ASSERT_EQ(thread.m_result, 0);
//   delete thread.m_object;

//   QuitThread thread2;
//   thread2.m_object = 0;
//   thread2.m_result = -1;
//   std::unique_lock<std::mutex> locker2(thread2.m_mutex);
//   thread2.start();
//   thread2.quit();
//   thread2.m_cond.wait(locker2);
//   ASSERT_TRUE(thread2.wait(five_minutes));
//   ASSERT_EQ(thread2.m_result, 0);
//}

//TEST(ThreadTest, testStarted)
//{
//   SimpleThread thread;
//   bool signalCatched = false;
//   thread.connectStartedSignal([&](){
//      signalCatched = true;
//   }, CoreApplication::getInstance(), pdk::ConnectionType::DirectConnection);
//   thread.start();
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(signalCatched);
//}

//TEST(ThreadTest, testFinished)
//{
//   SimpleThread thread;
//   bool signalCatched = false;
//   thread.connectFinishedSignal([&](){
//      signalCatched = true;
//   });
//   thread.start();
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(signalCatched);
//}

//TEST(ThreadTest, testTerminated)
//{
//   TerminateThread thread;
//   bool signalCatched = false;
//   thread.connectFinishedSignal([&](){
//      signalCatched = true;
//   });
//   {
//      std::unique_lock<std::mutex> locker(thread.m_mutex);
//      thread.start();
//      thread.m_cond.wait(locker);
//      thread.terminate();
//      thread.m_cond.notify_one();
//   }
//   ASSERT_TRUE(thread.wait(five_minutes));
//}

//TEST(ThreadTest, testExec)
//{
//   class MultipleExecThread : public Thread
//   {
//   public:
//      int m_res1;
//      int m_res2;

//      MultipleExecThread()
//         : m_res1(-2),
//           m_res2(-2)
//      {}

//      void run()
//      {
//         {
//            ExitObject o;
//            o.m_thread = this;
//            o.m_code = 1;
//            Timer::singleShot(100, [&]() {
//               o.slot();
//            });
//            m_res1 = exec();
//         }
//         {
//            ExitObject o;
//            o.m_thread = this;
//            o.m_code = 2;
//            Timer::singleShot(100, [&]() {
//               o.slot();
//            });
//            m_res2 = exec();
//         }
//      }
//   };
//   MultipleExecThread thread;
//   thread.start();
//   ASSERT_TRUE(thread.wait());
//   ASSERT_EQ(thread.m_res1, 1);
//   ASSERT_EQ(thread.m_res2, 2);
//}

//TEST(ThreadTest, testSleep)
//{
//   SleepThread thread;
//   thread.m_sleepType = SleepThread::Second;
//   thread.m_interval = 2;
//   thread.start();
//   ASSERT_TRUE(thread.wait(five_minutes));
//   ASSERT_TRUE(thread.m_elapsed >= 2000);
//}

//TEST(ThreadTest, testMSleep)
//{
//   SleepThread thread;
//   thread.m_sleepType = SleepThread::Millisecond;
//   thread.m_interval = 120;
//   thread.start();
//   ASSERT_TRUE(thread.wait(five_minutes));
//#if defined(PDK_OS_WIN)
//   // Since the resolution of Time is so coarse...
//   ASSERT_TRUE(thread.m_elapsed >= 100)
//      #else
//   ASSERT_TRUE(thread.m_elapsed >= 120);
//#endif
//}

//TEST(ThreadTest, testUSleep)
//{
//   SleepThread thread;
//   thread.m_sleepType = SleepThread::Microsecond;
//   thread.m_interval = 120000;
//   thread.start();
//   ASSERT_TRUE(thread.wait(five_minutes));
//#if defined(PDK_OS_WIN)
//   // Since the resolution of Time is so coarse...
//   ASSERT_TRUE(thread.m_elapsed >= 100)
//      #else
//   ASSERT_TRUE(thread.m_elapsed >= 120);
//#endif
//}

using FuncPtr = void (*)(void *);
void noop(void*) {}

#if defined PDK_OS_UNIX
using ThreadHandle = pthread_t;
#elif defined PDK_OS_WIN
using ThreadHandle = HANDLE;
#endif

#ifdef PDK_OS_WIN
#define WIN_FIX_STDCALL __stdcall
#else
#define WIN_FIX_STDCALL
#endif

class NativeThreadWrapper
{
public:
   NativeThreadWrapper()
      : m_pdkthread(0),
        m_waitForStop(false)
   {}
   
   void start(FuncPtr functionPointer = noop, void *data = nullptr);
   void startAndWait(FuncPtr functionPointer = noop, void *data = nullptr);
   void join();
   void setWaitForStop()
   {
      m_waitForStop = true;
   }
   
   void stop();
   
   ThreadHandle m_nativeThreadHandle;
   Thread *m_pdkthread;
   std::condition_variable m_startCondition;
   std::mutex m_mutex;
   bool m_waitForStop;
   std::condition_variable m_stopCondition;
protected:
   static void *runUnix(void *data);
   static unsigned WIN_FIX_STDCALL runWin(void *data);
   
   FuncPtr m_functionPointer;
   void *m_data;
};

void NativeThreadWrapper::start(FuncPtr functionPointer, void *data)
{
   this->m_functionPointer = functionPointer;
   this->m_data = data;
#if defined PDK_OS_UNIX
   const int state = pthread_create(&m_nativeThreadHandle, 0, NativeThreadWrapper::runUnix, this);
   PDK_UNUSED(state);
#elif defined(PDK_OS_WINRT)
   nativeThreadHandle = CreateThread(NULL, 0 , (LPTHREAD_START_ROUTINE)NativeThreadWrapper::runWin , this, 0, NULL);
#elif defined PDK_OS_WIN
   unsigned thrdid = 0;
   m_nativeThreadHandle = (pdk::HANDLE) _beginthreadex(NULL, 0, NativeThreadWrapper::runWin, this, 0, &thrdid);
#endif
}

void NativeThreadWrapper::startAndWait(FuncPtr functionPointer, void *data)
{
   std::unique_lock<std::mutex> locker(m_mutex);
   start(functionPointer, data);
   m_startCondition.wait(locker);
}

void NativeThreadWrapper::join()
{
#if defined PDK_OS_UNIX
   pthread_join(m_nativeThreadHandle, 0);
#elif defined PDK_OS_WIN
   WaitForSingleObjectEx(m_nativeThreadHandle, INFINITE, FALSE);
   CloseHandle(nativeThreadHandle);
#endif
}

void *NativeThreadWrapper::runUnix(void *that)
{
   NativeThreadWrapper *nativeThreadWrapper = reinterpret_cast<NativeThreadWrapper*>(that);
   // Adopt thread, create Thread object.
   nativeThreadWrapper->m_pdkthread = Thread::getCurrentThread();
   // Release main thread.
   {
      std::lock_guard<std::mutex> lock(nativeThreadWrapper->m_mutex);
      nativeThreadWrapper->m_startCondition.notify_one();
   }
   // Run function.
   nativeThreadWrapper->m_functionPointer(nativeThreadWrapper->m_data);
   // Wait for stop.
   {
      std::unique_lock<std::mutex> lock(nativeThreadWrapper->m_mutex);
      if (nativeThreadWrapper->m_waitForStop)
         nativeThreadWrapper->m_stopCondition.wait(lock);
   }
   return nullptr;
}

unsigned WIN_FIX_STDCALL NativeThreadWrapper::runWin(void *data)
{
   runUnix(data);
   return 0;
}

void NativeThreadWrapper::stop()
{
   std::lock_guard<std::mutex> lock(m_mutex);
   m_waitForStop = false;
   m_stopCondition.notify_one();
}

bool threadAdoptedOk = false;
Thread *mainThread;

void test_native_thread_adoption(void *)
{
   threadAdoptedOk = (Thread::getCurrentThreadId() != 0
         && Thread::getCurrentThread() != nullptr
         && Thread::getCurrentThread() != mainThread);
}

//TEST(ThreadTest, testNativeThreadAdoption)
//{
//   threadAdoptedOk = false;
//   mainThread = Thread::getCurrentThread();
//   NativeThreadWrapper nativeThread;
//   nativeThread.setWaitForStop();
//   nativeThread.startAndWait(test_native_thread_adoption);
//   ASSERT_TRUE(nativeThread.m_pdkthread);

//   nativeThread.stop();
//   nativeThread.join();

//   ASSERT_TRUE(threadAdoptedOk);
//}

//void adopted_thread_affinity_function(void *arg)
//{
//   Thread **affinity = reinterpret_cast<Thread **>(arg);
//   Thread *current = Thread::getCurrentThread();
//   affinity[0] = current;
//   affinity[1] = current->getThread();
//}

//TEST(ThreadTest, testAdoptedThreadAffinity)
//{
//   Thread *affinity[2] = { 0, 0 };
//   NativeThreadWrapper thread;
//   thread.startAndWait(adopted_thread_affinity_function, affinity);
//   thread.join();
//   // adopted thread should have affinity to itself
//   ASSERT_EQ(affinity[0], affinity[1]);
//}

//TEST(ThreadTest, testAdoptedThreadSetPriority)
//{
//   NativeThreadWrapper nativeThread;
//   nativeThread.setWaitForStop();
//   nativeThread.startAndWait();

//   // change the priority of a running thread
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::InheritPriority);
//   nativeThread.m_pdkthread->setPriority(Thread::IdlePriority);
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::IdlePriority);
//   nativeThread.m_pdkthread->setPriority(Thread::LowestPriority);
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::LowestPriority);
//   nativeThread.m_pdkthread->setPriority(Thread::LowPriority);
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::LowPriority);
//   nativeThread.m_pdkthread->setPriority(Thread::NormalPriority);
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::NormalPriority);
//   nativeThread.m_pdkthread->setPriority(Thread::HighPriority);
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::HighPriority);
//   nativeThread.m_pdkthread->setPriority(Thread::HighestPriority);
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::HighestPriority);
//   nativeThread.m_pdkthread->setPriority(Thread::TimeCriticalPriority);
//   ASSERT_EQ(nativeThread.m_pdkthread->getPriority(), Thread::TimeCriticalPriority);

//   nativeThread.stop();
//   nativeThread.join();
//}

//TEST(ThreadTest, testAdoptedThreadExit)
//{
//   NativeThreadWrapper nativeThread;
//   nativeThread.setWaitForStop();

//   nativeThread.startAndWait();
//   ASSERT_TRUE(nativeThread.m_pdkthread);
//   ASSERT_TRUE(nativeThread.m_pdkthread->isRunning());
//   ASSERT_TRUE(!nativeThread.m_pdkthread->isFinished());

//   nativeThread.stop();
//   nativeThread.join();
//}

//void adopted_thread_exec_function(void *)
//{
//   Thread  * const adoptedThread = Thread::getCurrentThread();
//   EventLoop eventLoop(adoptedThread);
//   const int code = 1;
//   ExitObject o;
//   o.m_thread = adoptedThread;
//   o.m_code = code;
//   Timer::singleShot(100, [&](){
//      o.slot();
//   });
//   const int result = eventLoop.exec();
//   ASSERT_EQ(result, code);
//}

//TEST(ThreadTest, testAdoptedThreadExec)
//{
//   NativeThreadWrapper nativeThread;
//   nativeThread.start(adopted_thread_exec_function);
//   nativeThread.join();
//}

//TEST(ThreadTest, testAdoptedThreadFinished)
//{
//   SimpleThread closureThread;
//   closureThread.start();
//   TestEventLoop *instance = &TestEventLoop::instance();
//   NativeThreadWrapper nativeThread;
//   nativeThread.setWaitForStop();
//   nativeThread.startAndWait();
//   nativeThread.m_pdkthread->connectFinishedSignal([&](){
//      std::cout << "xxxx" << std::endl;
//     // instance->exitLoop();
//   });


//   ASSERT_TRUE(closureThread.wait(five_minutes));
//   nativeThread.stop();
//   nativeThread.join();

//   TestEventLoop *temp = &TestEventLoop::instance();
//   temp->enterLoop(5);
//   ASSERT_TRUE(!TestEventLoop::instance().getTimeout());
//}

//TEST(ThreadTest, testStress)
//{
//   Time t;
//   t.start();
//   while (t.elapsed() < one_minute) {
//      CurrentThread t;
//      t.start();
//      t.wait(one_minute);
//   }
//}

class Syncronizer : public Object
{
public:
   PDK_DEFINE_SIGNAL_ENUMS(PropChanged);
   void setProp(int p) {
      if(m_prop != p) {
         m_prop = p;
         emitPropChangedSignal(std::move(p));
      }
   }
   void propChanged(int);
   ~Syncronizer()
   {
      
   }
   using PropChangedHandlerType = void(int);
   PDK_DEFINE_SIGNAL_BINDER(PropChanged)
   PDK_DEFINE_SIGNAL_EMITTER(PropChanged)
   
   public:
      Syncronizer() 
    : m_prop(42)
   {}
   
   int m_prop;
};

class CurrentThread1;

class CurrentThread1 : public Thread
{
public:
   pdk::HANDLE m_id;
   Thread *m_thread;
   
   void run()
   {
      exec();
   }
   
   void quit()
   {
      
      Thread::quit();
   }
   
   void propChanged(int val)
   {
      
   }
};

//TEST(ThreadTest, exitAndStart)
//{
//   CurrentThread1 thread;
//   thread.exit(555); //should do nothing
//   thread.start();
//   Syncronizer sync1;
//   sync1.moveToThread(&thread);
//   Syncronizer sync2;
//   sync2.moveToThread(&thread);
//   sync2.connectPropChangedSignal(&sync1, &Syncronizer::setProp, pdk::ConnectionType::QueuedConnection);
//   sync1.connectPropChangedSignal(&thread, &CurrentThread1::quit, pdk::ConnectionType::QueuedConnection);
//   CallableInvoker::invokeAsync([&sync2](int value) {
//      sync2.setProp(value);
//   }, &sync2, 89);

//   CoreApplication::getInstance()->getThread()->msleep(50);
//   while(!thread.wait(10)) {
//      CoreApplication::getInstance()->getThread()->msleep(10);
//   }

//   ASSERT_EQ(sync2.m_prop, 89);
//   ASSERT_EQ(sync1.m_prop, 89);
//}

//TEST(ThreadTest, testExitAndExec)
//{
//   class MyThread : public Thread {
//   public:
//      Semaphore m_sem1;
//      Semaphore m_sem2;
//      volatile int m_value;
//      void run() {
//         m_sem1.acquire();
//         m_value = exec();  //First entrence
//         m_sem2.release();
//         m_value = exec(); // Second loop
//      }
//   };
//   MyThread thread;
//   thread.m_value = 0;
//   thread.start();
//   thread.exit(556);
//   thread.m_sem1.release();
//   thread.m_sem2.acquire();
//   int v = thread.m_value;
//   ASSERT_EQ(v, 556);
//   Syncronizer sync1;
//   sync1.moveToThread(&thread);
//   Syncronizer sync2;
//   sync2.moveToThread(&thread);
//   sync2.connectPropChangedSignal(&sync1, &Syncronizer::setProp, pdk::ConnectionType::QueuedConnection);
//   sync1.connectPropChangedSignal(&thread, &MyThread::quit, pdk::ConnectionType::QueuedConnection);
//   CallableInvoker::invokeAsync([&sync2](int value) {
//      sync2.setProp(value);
//   }, &sync2, 89);

//   CoreApplication::getInstance()->getThread()->msleep(50);
//   while(!thread.wait(10)) {
//      CoreApplication::getInstance()->getThread()->msleep(10);
//   }
//   ASSERT_EQ(sync2.m_prop, 89);
//   ASSERT_EQ(sync1.m_prop, 89);
//}

//TEST(ThreadTest, testConnectThreadFinishedSignalToObjectDeleteLaterSlot)
//{
//   Thread thread;
//   Object *object = new Object;
//   Pointer<Object> p = object;
//   ASSERT_TRUE(!p.isNull());
//   thread.connectStartedSignal(&thread, &Thread::quit, pdk::ConnectionType::DirectConnection);
//   thread.connectFinishedSignal(object, &Object::deleteLater);
//   object->moveToThread(&thread);
//   thread.start();
//   ASSERT_TRUE(thread.wait(30000));
//   ASSERT_TRUE(p.isNull());
//}

class WaitingThread : public Thread
{
public:
   enum { WaitTime = 800 };
   std::mutex m_mutex;
   std::condition_variable m_cond1;
   std::condition_variable m_cond2;
   
   void run()
   {
      std::unique_lock locker(m_mutex);
      m_cond1.wait(locker);
      m_cond2.wait_for(locker, std::chrono::milliseconds(WaitTime));
   }
};

//TEST(ThreadTest, testWait2)
//{
//   ElapsedTimer timer;
//   WaitingThread thread;
//   thread.start();
//   timer.start();
//   ASSERT_TRUE(!thread.wait(WaitingThread::WaitTime));
//   pdk::pint64 elapsed = timer.elapsed();
//   ASSERT_TRUE(elapsed >= WaitingThread::WaitTime - 10) << pdk_printable(String::fromLatin1("elapsed: %1").arg(elapsed));
//   timer.start();
//   thread.m_cond1.notify_one();
//   ASSERT_TRUE(thread.wait(/*WaitingThread::WaitTime * 1.4*/));
//   elapsed = timer.elapsed();
//   ASSERT_TRUE(elapsed - WaitingThread::WaitTime >= -1) << pdk_printable(String::fromLatin1("elapsed: %1").arg(elapsed));
//}

class SlowSlotObject : public Object {
public:
   std::mutex m_mutex;
   std::condition_variable m_cond;
   void slowSlot(Thread::SignalType signal, Object *sender)
   {
      std::unique_lock locker(m_mutex);
      m_cond.wait(locker);
   }
};

//TEST(ThreadTest, testWait3SlowDestructor)
//{
//   SlowSlotObject slow;
//   Thread thread;
//   thread.connectFinishedSignal(&slow, &SlowSlotObject::slowSlot, pdk::ConnectionType::DirectConnection);
//   thread.connectFinishedSignal([](Thread::SignalType signal, Object *sender) {}, &slow, pdk::ConnectionType::DirectConnection);
//   enum { WaitTime = 1800 };
//   ElapsedTimer timer;
//   thread.start();
//   thread.quit();
//   timer.start();
//   ASSERT_TRUE(!thread.wait(WaitingThread::WaitTime));
//   pdk::pint64 elapsed = timer.elapsed();
//   ASSERT_TRUE(elapsed >= WaitingThread::WaitTime - 1) << pdk_printable(String::fromLatin1("elapsed: %1").arg(elapsed));
//   slow.m_cond.notify_one();
//   //now the thread should finish quickly
//   ASSERT_TRUE(thread.wait(one_minute));
//}

//TEST(ThreadTest, testDestroyFinishRace)
//{
//   class MyThread : public Thread { void run() {} };
//   for (int i = 0; i < 15; i++) {
//      MyThread *thread = new MyThread;
//      thread->connectFinishedSignal(thread, &MyThread::deleteLater);
//      Pointer<Thread> weak(static_cast<Thread *>(thread));
//      thread->start();
//      while (weak) {
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//      }
//   }
//}

//TEST(ThreadTest, testStartFinishRace)
//{
//   class MyThread : public Thread {
//   public:
//      MyThread() : m_i (50) {}
//      void run() {
//         m_i--;
//         if (!m_i) {
//            disconnectFinishedSignal(m_connection);
//         }
//      }
//      void start()
//      {
//         Thread::start(Priority::InheritPriority);
//      }
//      int m_i;
//      pdk::kernel::signal::Connection m_connection;
//   };
//   for (int i = 0; i < 15; i++) {
//      MyThread thread;
//      thread.m_connection = thread.connectFinishedSignal(&thread, &MyThread::start);
//      thread.start();
//      while (!thread.isFinished() || thread.m_i != 0) {
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//         PDK_RETRIEVE_APP_INSTANCE()->processEvents();
//      }
//      ASSERT_EQ(thread.m_i, 0);
//   }
//}

//TEST(ThreadTest, testStartAndQuitCustomEventLoop)
//{
//   struct MyThread : Thread {
//      void run() { EventLoop().exec(); }
//   };

//   for (int i = 0; i < 5; i++) {
//      MyThread t;
//      t.start();
//      t.quit();
//      t.wait();
//   }
//}

class FinishedTestObject : public Object {
public:
   FinishedTestObject()
      : m_ok(false)
   {}
   bool m_ok;
public:
   void slotFinished(Thread::SignalType signal, Object *sender)
   {
      Thread *t = dynamic_cast<Thread *>(sender);
      m_ok = t && t->isFinished() && !t->isRunning();
   }
};

TEST(ThreadTest, testIsRunningInFinished)
{
   for (int i = 0; i < 15; i++) {
      Thread thread;
      thread.start();
      FinishedTestObject localObject;
      FinishedTestObject inThreadObject;
      localObject.setObjectName(Latin1String("..."));
      inThreadObject.moveToThread(&thread);
      thread.connectFinishedSignal(&localObject, &FinishedTestObject::slotFinished);
      thread.connectFinishedSignal(&inThreadObject, &FinishedTestObject::slotFinished);
      EventLoop loop;
      thread.connectFinishedSignal(&loop, &EventLoop::quit);
      CallableInvoker::invokeAsync(&thread, &Thread::quit);
      loop.exec();
      ASSERT_TRUE(!thread.isRunning());
      ASSERT_TRUE(thread.isFinished());
      ASSERT_TRUE(localObject.m_ok);
      ASSERT_TRUE(inThreadObject.m_ok);
   }
}

namespace pdk {
namespace kernel {
PDK_CORE_EXPORT uint global_posted_events_count();
} // kernel
} // pdk

using pdk::kernel::global_posted_events_count;

using pdk::kernel::AbstractEventDispatcher;
using pdk::kernel::SocketNotifier;
using pdk::os::thread::BasicAtomicInt;

class DummyEventDispatcher : public AbstractEventDispatcher
{
public:
   DummyEventDispatcher() : AbstractEventDispatcher()
   {}
   
   bool processEvents(EventLoop::ProcessEventsFlags)
   {
      m_visited.store(true);
      emitAwakeSignal();
      CoreApplication::sendPostedEvents();
      return false;
   }
   
   bool hasPendingEvents()
   {
      return global_posted_events_count();
   }
   
   void registerSocketNotifier(SocketNotifier *)
   {}
   
   void unregisterSocketNotifier(SocketNotifier *)
   {}
   
   void registerTimer(int, int, pdk::TimerType, Object *)
   {}
   
   bool unregisterTimer(int )
   {
      return false;
   }
   
   bool unregisterTimers(Object *)
   {
      return false;
   }
   
   std::list<TimerInfo> getRegisteredTimers(Object *) const
   {
      return std::list<TimerInfo>();
   }
   
   int remainingTime(int)
   {
      return 0;
   }
   
   void wakeUp()
   {}
   
   void interrupt()
   {}
   
   void flush()
   {}
   
   //#ifdef PDK_OS_WIN
   //    bool registerEventNotifier(WinEventNotifier *) { return false; }
   //    void unregisterEventNotifier(WinEventNotifier *) { }
   //#endif
   
   BasicAtomicInt m_visited; // bool
};


class ThreadObj : public Object
{
public:
   using VisitedHandlerType = void();
   PDK_DEFINE_SIGNAL_ENUMS(Visited);
   PDK_DEFINE_SIGNAL_EMITTER(Visited)
   PDK_DEFINE_SIGNAL_BINDER(Visited)
   
   void visit()
   {
      emitVisitedSignal();
   }
public:
   void visited();
};


TEST(ThreadTest, testCustomEventDispatcher)
{
   Thread thread;
   // there should be no ED yet
   ASSERT_TRUE(!thread.getEventDispatcher());
   DummyEventDispatcher *ed = new DummyEventDispatcher;
   thread.setEventDispatcher(ed);
   // the new ED should be set
   ASSERT_EQ(thread.getEventDispatcher(), ed);
   // test the alternative API of AbstractEventDispatcher
   ASSERT_EQ(AbstractEventDispatcher::getInstance(&thread), ed);
   thread.start();
   // start() should not overwrite the ED
   ASSERT_EQ(thread.getEventDispatcher(), ed);
   
   ThreadObj obj;
   obj.moveToThread(&thread);
   // move was successful?
   ASSERT_EQ(obj.getThread(), &thread);
   EventLoop loop;
   obj.connectVisitedSignal(&loop, &EventLoop::quit, pdk::ConnectionType::QueuedConnection);
   CallableInvoker::invokeAsync([&obj]() {
      obj.visit();
   }, &obj);
   loop.exec();
   // test that the ED has really been used
   ASSERT_TRUE(ed->m_visited.load());
   
   Pointer<DummyEventDispatcher> weak_ed(ed);
   ASSERT_TRUE(!weak_ed.isNull());
   thread.quit();
   // wait for thread to be stopped
   ASSERT_TRUE(thread.wait(30000));
   // test that ED has been deleted
   ASSERT_TRUE(weak_ed.isNull());
}


int main(int argc, char **argv)
{
   CoreApplication app(argc, argv);
   int retCode = 0;
   ::testing::InitGoogleTest(&argc, argv);
   CallableInvoker::invokeAsync([&retCode]() {
      retCode = RUN_ALL_TESTS();
   }, &app);
   app.exec();
   return retCode;
}