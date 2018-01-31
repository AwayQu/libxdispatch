/*
* xdispatch_internal.h
*
* Copyright (c) 2011-2018 MLBA-Team
* All rights reserved.
*
* @LICENSE_HEADER_START@
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
* @LICENSE_HEADER_END@
*/


#ifndef XDISPATCH_INTERNAL_H_
#define XDISPATCH_INTERNAL_H_


#include <dispatch/dispatch.h>

#ifndef NSEC_PER_MSEC
    #define NSEC_PER_MSEC 1000000ll
#endif

#include <assert.h>
#include <string>

#define __XDISPATCH_BEGIN_NAMESPACE \
    namespace xdispatch \
    {
#define __XDISPATCH_END_NAMESPACE \
    }
#define __XDISPATCH_USE_NAMESPACE using namespace xdispatch; // NOLINT(build/namespaces)

#ifndef __XDISPATCH_INDIRECT__
    #define __XDISPATCH_INDIRECT__
#endif

#include "../include/xdispatch/platform.h"

#if XDISPATCH_COMPILER_MSVC2010 || XDISPATCH_COMPILER_MSVC2008SP1
    #pragma warning(disable: 4251) /* disable warning C4251 - * requires dll-interface */
    #define XDISPATCH_EXPORT __declspec( dllexport )
    #define XDISPATCH_DEPRECATED( F ) __declspec( deprecated ) F
#elif XDISPATCH_COMPILER_GCC || XDISPATCH_COMPILER_CLANG
    #define XDISPATCH_EXPORT __attribute__( ( __visibility__( "default" ) ) )
    #define XDISPATCH_DEPRECATED( F ) __attribute__ ( ( __deprecated__ ) ) F
#endif // if XDISPATCH_COMPILER_MSVC2010 || XDISPATCH_COMPILER_MSVC2008SP1

#include <assert.h>
#include <stdexcept>
#define XDISPATCH_ASSERT( X ) \
    { if( !( X ) ) /* NOLINT(readability/braces) */ \
      { \
          std::cerr << "Assertion failed: " #X " (at " << __FILE__ ":" << __LINE__ << ")" << std::endl; \
          std::terminate(); \
      } \
    }

#include "../include/xdispatch/pointer.h"
#include "../include/xdispatch/blocks.h"
#include "../include/xdispatch/base.h"
#include "../include/xdispatch/operation.h"
#include "../include/xdispatch/semaphore.h"
#include "../include/xdispatch/synchronized.h"
#include "../include/xdispatch/once.h"
#include "../include/xdispatch/queue.h"
#include "../include/xdispatch/group.h"
#include "../include/xdispatch/source.h"
#include "../include/xdispatch/timer.h"
#include "../include/xdispatch/lambda_dispatch.h"

#include "../../libdispatch/platform/platform.h"
#include "../../libdispatch/src/shims/hardware.h"

#include "execution.h"

#undef __XDISPATCH_INDIRECT__

#endif /* XDISPATCH_INTERNAL_H_ */
