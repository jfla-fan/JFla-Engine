#pragma once

// Global parameters

// 1 if Umbrella Engine is in debug mode
#define JF_DEBUG 1

// 1 if Umbrella Engine utilizes custom proxies for OpenImageIO library
#define JF_SUPPORT_OIIO_CUSTOM_PROXY 0



#include "Common/Common.h"
#include "Common/Compiler.h"
#include "Common/PlatformType.h"
#include "Common/BaseTypes.h"
#include "Common/AdvancedTypes.h"
#include "Common/Atomic.h"
#include "Common/ThreadCommon.h"
#include "Common/Traits.h"
#include "Common/Limits.h"
#include "Common/Assert.h"
#include "Common/Macro.h"


#include "Memory/Memory.h"
#include "Utils/FileSystem/FileSystem.h"
#include "STL/Containers.h"
#include "STL/Algorithms.h"
#include "STL/String.h"