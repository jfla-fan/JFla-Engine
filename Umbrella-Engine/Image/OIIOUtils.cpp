#include "OIIOUtils.h"


OIIO_NAMESPACE_BEGIN

namespace Filesystem
{
	IOFileSTLProxy::IOFileSTLProxy(string_view filename, Mode mode)
		: IOProxy(filename, mode)
	{

	}

}

OIIO_NAMESPACE_END