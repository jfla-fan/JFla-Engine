#pragma once
#include "GpuTypes.h"




/************************************************************************/
/*						GPU API INTERFACE                               */
/************************************************************************/

namespace UE::Graphics::GpuApi
{
	//class CTexture;
	//class CVertexBuffer;
	//class CIndexBuffer;
	//class CShader;


	SDeviceData& GetDeviceData();

	bool		 InitDevice();

	EDeviceType	 GetDeviceType();

	EGPUVendor	 GetGPUVendor();

}

