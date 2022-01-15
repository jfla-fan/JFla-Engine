#pragma once
#include <string>
#include <map>
#include <iterator>
#include <filesystem>
#include "OpenGLUtils.h"
#include "../Shader.h"



namespace J::Graphics
{
	using namespace J::Math;

	using GpuApi::EShaderType;



	static constexpr std::array<GLenum, 5> ToGLShaderTypeArray =
	{
		GL_VERTEX_SHADER,
		GL_TESS_CONTROL_SHADER,
		GL_COMPUTE_SHADER,
		GL_GEOMETRY_SHADER,
		GL_FRAGMENT_SHADER,
	};

	struct SPipelineStageInfo
	{
		EShaderType PipelineStage;
		std::string Source;
		JShader::IdType StageResource;
	};

	class OpenGLShader final : public JShader
	{
	public:

		using JShader::IdType;
		using JShader::UniformIdType;

		static constexpr IdType			InvalidShaderId = 0;
		static constexpr UniformIdType	InvalidUniformLocation = -1;

	private:

		static IdType CurrentlyAttachedShader;

		IdType Resource;

	public:

		OpenGLShader();
		~OpenGLShader() override;

		OpenGLShader(const OpenGLShader& another) = delete;
		OpenGLShader& operator = (const OpenGLShader& another) = delete;
		OpenGLShader(OpenGLShader&& another) NOEXCEPT;
		OpenGLShader& operator = (OpenGLShader&& another) NOEXCEPT;

		void Bind() const override;
		void Unbind() const override;

		IdType	GetHandle() const override { return Resource; }
		void	SetHandler(IdType resource) { this->Resource = resource; }
		bool	IsValid() const NOEXCEPT { return Resource != InvalidShaderId; }

		void	Release();


		static IdType CreateShaderProgram(SPipelineStageInfo* stages, SIZE_T count);

		template<std::ranges::forward_range _Range>
			requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, SPipelineStageInfo>
		static IdType CreateShaderProgram(_Range&& range);


		static IdType GetCurrentlyAttachedShader() { return CurrentlyAttachedShader; }
		
		// loaders

		void LoadFromString( const std::string& vertex, const std::string& fragment ) override;
		
		// ! todo implement those later !
		// ! possibly create preprocessor entity to process shader sources !

		void LoadFromString( const std::string& vertex, const std::string& geometry, const std::string& fragment ) override;

		void Load( const system::FilePath& vertexPath, const system::FilePath& fragmentPath ) override;
		void Load( const system::FilePath& vertexPath, const system::FilePath& geometry, const system::FilePath& fragmentPath ) override;

		// uniforms

		/**
		 * Retrieves location of a uniform variable in the current program object.
		 */
		UniformIdType GetUniformLocation( const CHAR* name ) const;
		UniformIdType GetUniformLocation( const std::string& name ) const;

		void SetUniform( const std::string& name, float		 value ) const override;
		void SetUniform( const std::string& name, int		 value ) const override;
		void SetUniform( const std::string& name, double	 value ) const override;
		void SetUniform( const std::string& name, Vector1	 value ) const override;
		void SetUniform( const std::string& name, Vector2	 value ) const override;
		void SetUniform( const std::string& name, Vector3	 value ) const override;
		void SetUniform( const std::string& name, Vector4	 value ) const override;
		void SetUniform( const std::string& name, VectorInt1 value ) const override;
		void SetUniform( const std::string& name, VectorInt2 value ) const override;
		void SetUniform( const std::string& name, VectorInt3 value ) const override;
		void SetUniform( const std::string& name, VectorInt4 value ) const override;

		// #todo implement later
		void SetUniform( const std::string& name, Matrix2x2	value ) const;
		void SetUniform( const std::string& name, Matrix3x2	value ) const;
		void SetUniform( const std::string& name, Matrix2x3	value ) const;
		void SetUniform( const std::string& name, Matrix3x3	value ) const;
		void SetUniform( const std::string& name, Matrix2x4	value ) const;
		void SetUniform( const std::string& name, Matrix4x2	value ) const;
		void SetUniform( const std::string& name, Matrix4x3	value ) const;
		void SetUniform( const std::string& name, Matrix3x4	value ) const;
		void SetUniform( const std::string& name, Matrix4x4	value ) const;


	protected:

		static IdType CreateShader(EShaderType type, const std::string& Source);

		static IdType CreateProgram(IdType* ids, SIZE_T shaderCount);

		static IdType CreateProgram(IdType id) { return CreateProgram(&id, 1); }

		template<std::forward_iterator _Iter> requires std::is_same_v<typename _Iter::value_type, OpenGLShader::IdType>
		static IdType CreateProgram(_Iter first, _Iter last);

		template<std::ranges::forward_range _Range>
			 requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, OpenGLShader::IdType>
		static IdType CreateProgram(_Range&& range);
	
	};


	template<std::ranges::forward_range _Range>
		requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, SPipelineStageInfo>
	OpenGLShader::IdType OpenGLShader::CreateShaderProgram(_Range&& range)
	{
		for (auto& stageInfo : range)
		{
			stageInfo.StageResource = OpenGLShader::CreateShader(stageInfo.PipelineStage, stageInfo.Source);
		}

		auto program = OpenGLShader::CreateProgram(std::views::transform(range, [](SPipelineStageInfo& stageInfo) -> IdType { return stageInfo.StageResource; }));
	
		for (auto& stageInfo : range)
		{
			OpenGLContext::DeleteShader(stageInfo.StageResource);
		}

		return program;
	}


	template<std::forward_iterator _Iter> requires std::is_same_v<typename _Iter::value_type, OpenGLShader::IdType>
	OpenGLShader::IdType OpenGLShader::CreateProgram(_Iter first, _Iter last)
	{
		IdType program = OpenGLContext::CreateProgram();

		for (auto it = first; it != last; ++it)
		{
			OpenGLContext::AttachShader(program, *it);
		}

		OpenGLContext::LinkProgram(program);
		if (LogGLProgramErrors(program, GL_LINK_STATUS))
		{
			// #todo WARN ABOUT LINKING FAILURE WITH
		}

		OpenGLContext::ValidateProgram(program);
		if (LogGLProgramErrors(program, GL_VALIDATE_STATUS))
		{
			// #todo WARN ABOUT VALIDATION FAILURE WITH
		}


		for (auto it = first; it != last; ++it)
		{
			OpenGLContext::DetachShader(program, *it);
		}

		return program;
	}


	template<std::ranges::forward_range _Range>
		requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, OpenGLShader::IdType>
	OpenGLShader::IdType OpenGLShader::CreateProgram(_Range&& range)
	{
		IdType program = OpenGLContext::CreateProgram();

		for (auto resource : range)
		{
			OpenGLContext::AttachShader(program, resource);
		}

		OpenGLContext::LinkProgram(program);
		if (LogGLProgramErrors(program, GL_LINK_STATUS))
		{
			// #todo WARN ABOUT LINKING FAILURE
		}

		OpenGLContext::ValidateProgram(program);
		if (LogGLProgramErrors(program, GL_VALIDATE_STATUS))
		{
			// #todo WARN ABOUT VALIDATION FAILURE
		}

		for (auto resource : range)
		{
			OpenGLContext::DetachShader(program, resource);
		}

		return program;
	}


}

