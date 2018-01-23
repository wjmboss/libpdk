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
// Created by softboy on 2018/01/21.

#include "gtest/gtest.h"
#include "pdk/kernel/signal/Signal.h"
#include <optional>
#include <functional>
#include <iostream>
#include <typeinfo>

namespace {

template<typename T>
struct MaxOrDefault {
   typedef T ResultType;
   using result_type = ResultType;
   template<typename InputIterator>
   typename InputIterator::ValueType
   operator()(InputIterator first, InputIterator last) const
   {
      std::optional<T> max;
      for (; first != last; ++first) {
         try
         {
            if(!max) max = *first;
            else max = (*first > max.value())? *first : max;
         }
         catch(const std::bad_weak_ptr &)
         {}
      }
      if(max) return max.value();
      return T();
   }
};

struct MakeInt {
   MakeInt(int n, int cn) : N(n), CN(cn) {}
   
   int operator()()
   { 
      return N;
   }
   
   int operator()() const
   {
      return CN;
   }
   
   int N;
   int CN;
};

template<int N>
struct MakeIncreasingInt {
   MakeIncreasingInt() : n(N)
   {}
   
   int operator()() const
   {
      return n++;
   }
   
   mutable int n;
};

}

namespace Signals = pdk::kernel::signal;

TEST(SignalTest, testZeroArgs)
{
   MakeInt i42(42, 41);
   MakeInt i2(2, 1);
   MakeInt i72(72, 71);
   MakeInt i63(63, 63);
   MakeInt i62(62, 61);
   {
      Signals::Signal<int (), MaxOrDefault<int>> signal0;
      std::cout << "sizeof(signal) = " << sizeof(signal0) << std::endl;
      Signals::Connection conn2 = signal0.connect(i2);
      Signals::Connection conn72 = signal0.connect(72, i72);
      Signals::Connection conn62 = signal0.connect(60, i62);
      Signals::Connection conn42 = signal0.connect(42, i42);
      ASSERT_EQ(signal0(), 72);
      signal0.disconnect(72);
      ASSERT_EQ(signal0(), 62);
      // Double-disconnect should be safe
      signal0.disconnect(72);
      ASSERT_EQ(signal0(), 62);
      // Triple-disconect should be safe
      ASSERT_EQ(signal0(), 62);
      // Also connect 63 in the same group as 62
      signal0.connect(60, i63);
      ASSERT_EQ(signal0(), 63);
      // Disconnect all of the 60's
      signal0.disconnect(60);
      ASSERT_EQ(signal0(), 42);
      conn42.disconnect();
      ASSERT_EQ(signal0(), 2);
      conn2.disconnect();
      ASSERT_EQ(signal0(), 0);
   }
   {
      Signals::Signal<int (), MaxOrDefault<int>> signal0;
      Signals::Connection conn2 = signal0.connect(i2);
      Signals::Connection conn72 = signal0.connect(i72);
      Signals::Connection conn62 = signal0.connect(i62);
      Signals::Connection conn42 = signal0.connect(i42);
      const Signals::Signal<int (), MaxOrDefault<int>> &csiganl0 = signal0;
      ASSERT_EQ(csiganl0(), 72);
   }
   {
      MakeIncreasingInt<7> i7;
      MakeIncreasingInt<10> i10;
      Signals::Signal<int (), MaxOrDefault<int>> signal0;
      Signals::Connection conn7 = signal0.connect(i7);
      Signals::Connection conn10 = signal0.connect(i10);
      ASSERT_EQ(signal0(), 10);
      ASSERT_EQ(signal0(), 11);
      PDK_UNUSED(conn7);
      PDK_UNUSED(conn10);
   }
}

TEST(SignalTest, testOneArgs)
{
   Signals::Signal<int (int value), MaxOrDefault<int>> signal0;
   signal0.connect(std::negate<int>());
   signal0.connect(std::bind(std::multiplies<int>(), 2, std::placeholders::_1));
   ASSERT_EQ(signal0(1), 2);
   ASSERT_EQ(signal0(-1), 1);
   signal0.connect([](int value){
      return 100 + value;
   });
   ASSERT_EQ(signal0(2), 102);
}

TEST(SignalTest, testSignalSignalConnect)
{
   using SignalType = Signals::Signal<int (int value), MaxOrDefault<int>>;
   SignalType signal1;
   signal1.connect(std::negate<int>());
   ASSERT_EQ(signal1(3), -3);
   {
      SignalType signal2;
      signal1.connect(signal2);
      signal2.connect(std::bind(std::multiplies<int>(), 2, std::placeholders::_1));
      signal2.connect(std::bind(std::multiplies<int>(), -3, std::placeholders::_1));
//      ASSERT_EQ(signal2(-3), 9);
//      ASSERT_EQ(signal1(3), 6);
   }
   ASSERT_EQ(signal1(3), -3);
}
