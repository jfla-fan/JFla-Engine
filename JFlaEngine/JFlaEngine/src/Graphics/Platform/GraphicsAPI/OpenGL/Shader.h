#pragma once
#include "../../../../Core.h"



namespace Engine::Graphics
{
	// Shader builder

	template<class _Shader_type>
	class ShaderBuilder
	{
	public:

		virtual Scope<_Shader_type> LoadFrom(const std::string& Source) = 0;

	};
	

	// Shader class

	template<class _Derived_shader>
	class Shader
	{
	public:

		virtual ~Shader() { };

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		inline static Scope<_Derived_shader> LoadFrom(const std::string& Source)
		{
			return ShaderBuilder<_Derived_shader>().LoadFrom(Source);
		}

	private:

		friend class ShaderBuilder<_Derived_shader>;
	};

}

