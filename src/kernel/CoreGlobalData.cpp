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
// Created by softboy on 2018/02/01.

#include "pdk/kernel/internal/CoreGlobalDataPrivate.h"
#include "pdk/global/GlobalStatic.h"

namespace pdk {
namespace kernel {
namespace internal {

PDK_GLOBAL_STATIC(CoreGlobalData, sg_globalInstance);

CoreGlobalData::CoreGlobalData()
#if PDK_CONFIG(TEXT_CODEC)
   : m_codecForLocale(nullptr)
   #endif
{
}

CoreGlobalData::~CoreGlobalData()
{
#if PDK_CONFIG(TEXT_CODEC)
   m_codecForLocale = nullptr;
   for (std::list<TextCodec *>::const_iterator iter = m_allCodecs.cbegin(); iter != m_allCodecs.cend(); ++iter){
      delete *iter;
   }
#endif
}

CoreGlobalData *CoreGlobalData::getInstance()
{
   return sg_globalInstance();
}

} // internal
} // kernel
} // pdk
