#pragma once
#include "../../GpuTypes.h"
#include "../OpenGLContext.h"



namespace J::Graphics::GpuApi
{

	struct SDeviceData;

	// #TODO DECIDE ABOUT THIS
	// SHOULD IT BE PURE OPENGL IMPLEMENTATION OR IT IS BETTER JUST TO LEAVE IT HERE.
	// IF YOU CHOOSE THE FIRST VARIANT, GOTTA DEAL WITH COMPILIER ERROR C2709 
	// RESOURCE CONTAINERS IN DEVICE DATA ALLOCATE THEIR MEMORY ON STACK AND
	// REQUIRE SBufferData TO BE DEFINED. DECLARATION DOES NOT WORK AS WELL AS POINTERS.
	// WHAT A DILEMMA ...

	struct SBufferData
	{

		SBufferDesc Description;

		GLuint		Resource;		// GPU descriptor

		SBufferData()
			: Description()
			, Resource(0)
		{
		}

	};

	
	// STextureData, etc ...

}


