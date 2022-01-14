#pragma once
#include <OpenImageIO/filesystem.h>
//#include "../Core.h"


OIIO_NAMESPACE_BEGIN

namespace Filesystem
{ 

	/**
	* Proxy filesystem object that utilizes newest C++ standard.
	*/
	class IOFileSTLProxy : IOProxy
	{
		IOFileSTLProxy(string_view filename, Mode mode);
	};

}

OIIO_NAMESPACE_END