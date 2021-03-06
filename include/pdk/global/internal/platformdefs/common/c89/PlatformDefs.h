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
// Created by softboy on 2018/01/25.

#ifndef PDK_GLOBAL_INTERNAL_COMMON_C89_PLATFORM_DEFS_H
#define PDK_GLOBAL_INTERNAL_COMMON_C89_PLATFORM_DEFS_H

#define PDK_FPOS_T               fpos_t
#define PDK_OFF_T                long

#define PDK_FOPEN                ::fopen
#define PDK_FSEEK                ::fseek
#define PDK_FTELL                ::ftell
#define PDK_FGETPOS              ::fgetpos
#define PDK_FSETPOS              ::fsetpos

#endif // PDK_GLOBAL_INTERNAL_COMMON_C89_PLATFORM_DEFS_H
