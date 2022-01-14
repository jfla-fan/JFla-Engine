#include "OpenGLShader.h"
#include "OpenGLContext.h"
#include "OpenGLUtils.h"
#include "../GpuApi.h"
#include <vector>



namespace J::Graphics
{

	OpenGLShader::IdType OpenGLShader::CurrentlyAttachedShader = OpenGLShader::InvalidShaderId;

	OpenGLShader::OpenGLShader()
		: Resource(InvalidShaderId)
	{
	}

	OpenGLShader::OpenGLShader(OpenGLShader&& another) NOEXCEPT
		: Resource(another.Resource)
	{
	}

	OpenGLShader::~OpenGLShader()
	{
		this->Release();
	}


	OpenGLShader& OpenGLShader::operator = (OpenGLShader&& another) NOEXCEPT
	{
		this->Release();
		this->Resource = another.Resource;

		return *this;
	}

	OpenGLShader::IdType OpenGLShader::CreateShader(EShaderType shaderType, const std::string& Source)
	{
		IdType resource = OpenGLContext::CreateShader(GL_ENUM(ToGLShaderTypeArray[(SIZE_T)shaderType]));
	
		// todo perform some magic on source code using preprocessor entity (in future)
	
		const auto codeptr = Source.c_str();

		OpenGLContext::ShaderSource(resource, 1, &codeptr, nullptr);

		OpenGLContext::CompileShader(resource);
		if (LogGLShaderErrors(resource, GL_COMPILE_STATUS))
		{
			// todo WARN ABOUT COMPILATION FAILURE WITH A MACRO
		}

		return resource;
	}
	
	void OpenGLShader::Release()
	{
		if (this->Resource != InvalidShaderId)
		{
			if (this->Resource == CurrentlyAttachedShader)
			{
				CurrentlyAttachedShader = InvalidShaderId;
			}

			OpenGLContext::DeleteProgram(this->Resource);
		}

		this->Resource = InvalidShaderId;
	}


	OpenGLShader::IdType OpenGLShader::CreateProgram(IdType* ids, SIZE_T shaderCount)
	{
		IdType program = OpenGLContext::CreateProgram();

		for (int32 i = 0; i < shaderCount; ++i)
		{
			OpenGLContext::AttachShader(program, ids[i]);
		}


		OpenGLContext::LinkProgram(program);
		if (LogGLProgramErrors(program, GL_LINK_STATUS))
		{
			// todo WARN ABOUT LINKING FAILURE WITH A MACRO
		}

		
		OpenGLContext::ValidateProgram(program);
		if (LogGLProgramErrors(program, GL_VALIDATE_STATUS))
		{
			// todo WARN ABOUT VALIDATION FAILURE WITH A MACRO
		}


		for (int32 i = 0; i < shaderCount; ++i)
		{
			OpenGLContext::DetachShader(program, ids[i]);
		}

		return program;
	}


	void OpenGLShader::Bind() const
	{
		OpenGLContext::UseProgram(this->Resource);
		OpenGLShader::CurrentlyAttachedShader = this->Resource;
	}

	void OpenGLShader::Unbind() const
	{
		OpenGLContext::UseProgram(InvalidShaderId);
		OpenGLShader::CurrentlyAttachedShader = InvalidShaderId;
	}


	OpenGLShader::IdType OpenGLShader::CreateShaderProgram(SPipelineStageInfo* stages, SIZE_T count)
	{
		for (SIZE_T i = 0; i < count; ++i)
		{
			auto& [stage, code, resource] = stages[i];
			
			resource = OpenGLShader::CreateShader(stage, code);
		}

		
		// use allocator?
		IdType* ids = new IdType[count];

		for (int i = 0; i < count; ++i)
		{
			ids[i] = stages[i].StageResource;
		}


		IdType program = OpenGLShader::CreateProgram(ids, count);

		
		for (SIZE_T i = 0; i < count; ++i)
		{
			OpenGLContext::DeleteShader(stages[i].StageResource);
		}


		return program;
	}

	// loaders

	void OpenGLShader::LoadFromString(const std::string& vertex, const std::string& fragment)
	{
		std::array<SPipelineStageInfo, 2>  stages =
		{
			SPipelineStageInfo { EShaderType::VERTEX_SHADER, vertex, InvalidShaderId },
			SPipelineStageInfo { EShaderType::FRAGMENT_SHADER, fragment, InvalidShaderId },
		};

		auto program = CreateShaderProgram(stages);

		this->SetHandler(program);
	}

	// #todo implement later other loaders (with appropriate engine filesystem)

	void OpenGLShader::LoadFromString(const std::string& vertex, const std::string& geometry, const std::string& fragment)
	{
		throw "NotImplementedYet";
	}

	void OpenGLShader::Load(const system::FilePath& vertexPath, const system::FilePath& fragmentPath)
	{
		throw "NotImplementedYet";
	}

	void OpenGLShader::Load(const system::FilePath& vertexPath, const system::FilePath& geometry, const system::FilePath& fragmentPath)
	{
		throw "NotImplementedYet";
	}

	// uniforms

	OpenGLShader::UniformIdType OpenGLShader::GetUniformLocation(const CHAR* name) const
	{
		if (!IsValid())
		{
			// #todo Warn about it (when logging system will be completed)
		}

		return OpenGLContext::GetUniformLocation(this->Resource, name);
	}

	OpenGLShader::UniformIdType OpenGLShader::GetUniformLocation(const std::string& name) const
	{
		if (!IsValid())
		{
			// #todo Warn about it (when logging system will be implemented)
		}

		return OpenGLContext::GetUniformLocation(this->Resource, name.c_str());
	}

#define SET_UNIFORM_LOG(Location, UniformName, Value)\
do {\
if (Location == -1)\
{\
	std::cout << std::format("Warning: could not find {} at location {}.\n", Value, UniformName);\
	return;\
}} while(0)

	void OpenGLShader::SetUniform(const std::string& name, float value) const
	{
		IdType location = GetUniformLocation(name);

		SET_UNIFORM_LOG(location, name, value);

		OpenGLContext::Uniform1f(location, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, int value) const
	{
		auto location = GetUniformLocation(name);

		SET_UNIFORM_LOG(location, name, value);

		OpenGLContext::Uniform1i(location, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, double value) const
	{
		auto location = GetUniformLocation(name);

		SET_UNIFORM_LOG(location, name, value);

		OpenGLContext::Uniform1d(location, value);
	}

	void OpenGLShader::SetUniform(const std::string& name, Vector1 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform1f(location, value.x);
	}

	void OpenGLShader::SetUniform(const std::string& name, Vector2 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform2f(location, value.x, value.y);
	}
	
	void OpenGLShader::SetUniform(const std::string& name, Vector3 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform3f(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniform(const std::string& name, Vector4 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform4f(location, value.x, value.y, value.z, value.w);
	}

	void OpenGLShader::SetUniform(const std::string& name, VectorInt1 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform1i(location, value.x);
	}

	void OpenGLShader::SetUniform(const std::string& name, VectorInt2 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform2i(location, value.x, value.y);
	}

	void OpenGLShader::SetUniform(const std::string& name, VectorInt3 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform3i(location, value.x, value.y, value.z);
	}

	void OpenGLShader::SetUniform(const std::string& name, VectorInt4 value) const
	{
		auto location = GetUniformLocation(name);

		OpenGLContext::Uniform4i(location, value.x, value.y, value.z, value.w);
	}

#undef SET_UNIFORM_LOG
}
