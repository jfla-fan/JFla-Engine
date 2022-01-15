#pragma once
#include "../../../../Core.h"
#include "OpenGLShader.h"
#include <vector>


namespace J::Graphics
{
	
	// #todo Implement this
	class OpenGLPipeline
	{
	public:

		using IdType = OpenGLShader::IdType;

	private:

		std::vector<IdType> Programs;

		IdType Resource;

	public:

		OpenGLPipeline();
		OpenGLPipeline(OpenGLPipeline&& another);
		~OpenGLPipeline();


		void Bind() const;
		void Unbind() const;

	protected:

		void Release();

	};


}

