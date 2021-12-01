#pragma once
#include "../../../../Core.h"



namespace Engine::Graphics
{

	class Shader
	{
	public:
		using BindableId = int32;

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

	protected:



	private:


	};

}

