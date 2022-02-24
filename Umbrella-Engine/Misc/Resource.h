#pragma once
#include "../Common/BaseTypes.h"
#include "../Common/Macro.h"



namespace UE
{

	enum class EResourceType : uint32
	{
		GRAPHICS			= JF_BIT( 0 ),

			BUFFER			= GRAPHICS | JF_BIT( 1 ),
			TEXTURE			= GRAPHICS | JF_BIT( 2 ),
			SHADER			= GRAPHICS | JF_BIT( 3 ),
			VERTEX_ARRAY	= GRAPHICS | JF_BIT( 4 ),
			IMAGE			= GRAPHICS | JF_BIT( 5 ),

		RENDER				= GRAPHICS | JF_BIT( 6 ),

		MEMORY				= JF_BIT( 7 ),

		OTHER				= JF_BIT( 8 ),
			 
	};


	class JResource
	{
	public:

		JResource() { }

		virtual ~JResource() { }

		virtual EResourceType GetResourceType() = 0;

	};


}
