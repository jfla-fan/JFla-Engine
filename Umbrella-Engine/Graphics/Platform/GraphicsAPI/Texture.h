#pragma once
#include "GpuTypes.h"



namespace J::Graphics::GpuApi
{



	class JTexture 
	{
	public:

		JTexture() { }

		virtual ~JTexture() { }


		virtual bool IsValid() const = 0;

	};

}


