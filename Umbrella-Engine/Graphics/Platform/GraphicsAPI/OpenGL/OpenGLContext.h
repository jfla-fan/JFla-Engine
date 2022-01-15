#pragma once
#include "../../../../Core.h"
#include "glad/glad.h"

#define GL_ENUM(element) (GLenum)(element)

#ifdef JF_DEBUG

	#define GL_ERROR_CODE_STR(code) OpenGLContext::GetStrErrorCode(code)

	#define GLCALL(call) do														\
	{																			\
		call;																	\
		GLenum error = glGetError();											\
		bool has_error = error != GL_NO_ERROR;									\
		if (has_error)															\
			std::cout << "OpenGL error in " << #call << " instruction.\n";		\
		while (error != GL_NO_ERROR)											\
		{																		\
			std::cout << std::format("Error code - {}\n.",						\
										GL_ERROR_CODE_STR(error));				\
			error = glGetError();												\
		}																		\
		if (has_error) JF_DEBUG_BREAK();										\
	} while (0)

#else

	#define GLCALL(call) do { call; } while(0)

#endif




namespace J::Graphics
{

	// #TODO DO WE REALLY NEED THIS?
	struct OpenGLContext
	{
		constexpr static GLuint GInvalidGLResource = 0;

		/**
		 * POSSIBLY IMPLEMENT HERE OPENGL STATISTICS (INHERITED FROM GPU API STATISTICS ??? )
		 * 
		 * BindCalls
		 * GenCalls
		 * DeleteCalls
		 * Errors
		 * ShaderCalls
		 * 
		 * 
		 * etc...
		 */


		// constants

		static constexpr FORCEINLINE uint32 GetMaxVertexAttributesCount() { return GL_MAX_VERTEX_ATTRIBS; }

		// specific

		static constexpr const CHAR* GetStrErrorCode(GLenum error)
		{
#define CASE_LABEL(code)	\
		case code:			\
			return #code

			switch (error)
			{
				CASE_LABEL(GL_NO_ERROR);
				CASE_LABEL(GL_INVALID_ENUM);
				CASE_LABEL(GL_INVALID_VALUE);
				CASE_LABEL(GL_INVALID_OPERATION);
				CASE_LABEL(GL_INVALID_FRAMEBUFFER_OPERATION);
				CASE_LABEL(GL_OUT_OF_MEMORY);
				CASE_LABEL(GL_STACK_UNDERFLOW);
				CASE_LABEL(GL_STACK_OVERFLOW);

			default:
				return "[Unknown error]";
			}
#undef CASE_LABEL

		}
		

		/************************************************************************/
		/*							OPENGL API									*/
		/************************************************************************/

		static FORCEINLINE void ClearBufferfv(GLenum Buffer, GLint DrawBufferIndex, const GLfloat* value)
		{
			GLCALL(glClearBufferfv(Buffer, DrawBufferIndex, value));
		}

		static FORCEINLINE void ClearBufferfi(
														GLenum Buffer,
														GLint DrawBufferIndex,
														GLfloat Depth,
														GLint Stencil)
		{
			GLCALL(glClearBufferfi(Buffer, DrawBufferIndex, Depth, Stencil));
		}

		static FORCEINLINE void ClearBufferiv(GLenum Buffer, GLint DrawBufferIndex, const GLint* Value)
		{
			GLCALL(glClearBufferiv(Buffer, DrawBufferIndex, Value));
		}

		static FORCEINLINE void ClearDepth(GLdouble Depth)
		{
			GLCALL(glClearDepth(Depth));
		}

		static FORCEINLINE void ClearColor(GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat Alpha)
		{
			GLCALL(glClearColor(Red, Green, Blue, Alpha));
		}

		static FORCEINLINE void Clear(GLbitfield Mask)
		{
			GLCALL(glClear(Mask));
		}
		
		static FORCEINLINE void PointSize(GLfloat Size)
		{
			check(Size >= 1.f);
			GLCALL(glPointSize(Size));
		}

		// Buffers

		static FORCEINLINE void GenBuffers(GLsizei n, GLuint* buffers)
		{
			GLCALL(glGenBuffers(n, buffers));
		}

		static FORCEINLINE void CreateBuffers(GLsizei n, GLuint* buffers)
		{
			GLCALL(glCreateBuffers(n, buffers));
		}

		static FORCEINLINE void DeleteBuffers( GLsizei n, GLuint* buffers )
		{
			GLCALL( glDeleteBuffers( n, buffers ) );
		}

		static FORCEINLINE void BindBuffer( GLenum Target, GLuint Buffer )
		{
			GLCALL( glBindBuffer( Target, Buffer ) );
		}

		static FORCEINLINE void UnbindBuffer( GLenum Target )
		{
			GLCALL( glBindBuffer( Target, GInvalidGLResource ) );
		}

		static FORCEINLINE void BufferData( GLenum Target,
											GLsizeiptr Size,
											CMemPtr Data,
											GLenum Usage )
		{
			GLCALL( glBufferData( Target, Size, Data, Usage ) );
		}

		static FORCEINLINE void NamedBufferData( GLuint Buffer,
												 GLsizeiptr Size,
												 CMemPtr Data,
												 GLenum Usage )
		{
			GLCALL( glNamedBufferData( Buffer, Size, Data, Usage ) );
		}

		static FORCEINLINE void CopyBufferSubData( GLenum ReadTarget,
												   GLenum WriteTarget,
												   GLintptr ReadOffset,
												   GLintptr WriteOffset,
												   GLsizeiptr Size )
		{
			GLCALL( glCopyBufferSubData( ReadTarget, WriteTarget, ReadOffset, WriteOffset, Size ) );
		}

		static FORCEINLINE void CopyNamedBufferSubData( GLuint ReadBuffer,
														GLuint WriteBuffer,
														GLintptr ReadOffset,
														GLintptr WriteOffset,
														GLsizeiptr Size )
		{
			GLCALL( glCopyNamedBufferSubData( ReadBuffer, WriteBuffer, ReadOffset, WriteOffset, Size ) );
		}

		static FORCEINLINE void BufferSubData( GLenum Target,
							GLintptr Offset,
							GLsizeiptr Size,
							CMemPtr Data )
		{
			GLCALL( glBufferSubData( Target, Offset, Size, Data ) );
		}

		static FORCEINLINE void NamedBufferSubData( GLuint Buffer,
								 GLintptr Offset,
								 GLsizeiptr Size,
								 CMemPtr Data )
		{
			GLCALL( glNamedBufferSubData( Buffer, Offset, Size, Data ) );
		}

		static FORCEINLINE void GetBufferSubData( GLenum Target,
							   GLintptr Offset,
							   GLsizeiptr Size,
							   MemPtr Data )
		{
			GLCALL( glGetBufferSubData( Target, Offset, Size, Data ) );
		}

		static FORCEINLINE void GetNamedBufferSubData(GLuint Buffer,
									GLintptr Offset,
									GLsizeiptr Size,
									MemPtr Data )
		{
			GLCALL( glGetNamedBufferSubData( Buffer, Offset, Size, Data ) );
		}

		static FORCEINLINE void BufferStorage( GLenum Target,
											   GLsizeiptr Size,
											   CMemPtr Data,
											   GLbitfield Flags )
		{
			GLCALL( glBufferStorage( Target, Size, Data, Flags ) );
		}

		static FORCEINLINE void NamedBufferStorage( GLuint Buffer,
													GLsizeiptr Size,
													CMemPtr Data,
													GLbitfield Flags )
		{
			GLCALL( glNamedBufferStorage( Buffer, Size, Data, Flags ) );
		}


		// Shaders

		static FORCEINLINE GLuint CreateShader(GLenum Type)
		{
			GLuint resource;
			GLCALL(resource = glCreateShader(Type));
		
			return resource;
		}

		static FORCEINLINE void ShaderSource(
														GLuint Shader,
														GLsizei Count,
														const GLchar* const *String,
														const GLint* Length)
		{
			GLCALL(glShaderSource(Shader, Count, String, Length));
		}

		static FORCEINLINE void CompileShader( GLuint Shader )
		{
			GLCALL( glCompileShader( Shader ) );
		}

		static FORCEINLINE void GetShaderiv( GLuint Shader, GLenum ParameterName, GLint* Value )
		{
			GLCALL(glGetShaderiv(Shader, ParameterName, Value));
		}

		static FORCEINLINE void GetShaderInfoLog( GLuint Shader,
												  GLsizei BufferSize,
												  GLsizei* Length,
												  GLchar* InfoLogBuffer )
		{
			GLCALL(glGetShaderInfoLog(Shader, BufferSize, Length, InfoLogBuffer));
		}

		static FORCEINLINE void DeleteShader(GLuint Shader)
		{
			GLCALL(glDeleteShader(Shader));
		}

		static FORCEINLINE void DetachShader(GLuint Program, GLuint Shader)
		{
			GLCALL(glDetachShader(Program, Shader));
		}

		// uniforms

		static FORCEINLINE GLint GetUniformLocation(GLuint Program, const GLchar* Name)
		{
			GLint result;
			GLCALL(result = glGetUniformLocation(Program, Name));
			
			return result;
		}

		static FORCEINLINE void Uniform1f(GLint location, float v0)
		{
			GLCALL(glUniform1f(location, v0));
		}

		static FORCEINLINE void Uniform2f(GLint location, float v0, float v1)
		{
			GLCALL(glUniform2f(location, v0, v1));
		}

		static FORCEINLINE void Uniform3f(GLint location, float v0, float v1, float v2)
		{
			GLCALL(glUniform3f(location, v0, v1, v2));
		}

		static FORCEINLINE void Uniform4f(GLint location, float v0, float v1, float v2, float v3)
		{
			GLCALL(glUniform4f(location, v0, v1, v2, v3));
		}

		static FORCEINLINE void Uniform1i(GLint location, GLint v0)
		{
			GLCALL(glUniform1i(location, v0));
		}

		static FORCEINLINE void Uniform2i(GLint location, GLint v0, GLint v1)
		{
			GLCALL(glUniform2i(location, v0, v1));
		}

		static FORCEINLINE void Uniform3i(GLint location, GLint v0, GLint v1, GLint v2)
		{
			GLCALL(glUniform3i(location, v0, v1, v2));
		}

		static FORCEINLINE void Uniform4i(GLint location, GLint v0, GLint v1, GLint v2, GLint v3)
		{
			GLCALL(glUniform4i(location, v0, v1, v2, v3));
		}

		static FORCEINLINE void Uniform1d(GLint location, GLdouble v0)
		{
			GLCALL(glUniform1d(location, v0));
		}

		static FORCEINLINE void Uniform2d(GLint location, GLdouble v0, GLdouble v1)
		{
			GLCALL(glUniform2d(location, v0, v1));
		}

		static FORCEINLINE void Uniform3d(GLint location, GLdouble v0, GLdouble v1, GLdouble v2)
		{
			GLCALL(glUniform3d(location, v0, v1, v2));
		}

		static FORCEINLINE void Uniform4d(GLint location, GLdouble v0, GLdouble v1, GLdouble v2, GLdouble v3)
		{
			GLCALL(glUniform4d(location, v0, v1, v2, v3));
		}

		static FORCEINLINE void UniformMatrix2fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix2fv(location, count, transpose, value));
		}

		static FORCEINLINE void UniformMatrix2x3fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix2x3fv(location, count, transpose, value));
		}
		
		static FORCEINLINE void UniformMatrix3x2fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix3x2fv(location, count, transpose, value));
		}
		
		static FORCEINLINE void UniformMatrix4fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix4fv(location, count, transpose, value));
		}

		static FORCEINLINE void UniformMatrix3x4fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix3x4fv(location, count, transpose, value));
		}

		static FORCEINLINE void UniformMatrix4x3fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix4x3fv(location, count, transpose, value));
		}

		static FORCEINLINE void UniformMatrix2x4fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix2x4fv(location, count, transpose, value));
		}

		static FORCEINLINE void UniformMatrix4x2fv(
													GLint location,
													GLsizei count,
													GLboolean transpose,
													const GLfloat* value)
		{
			GLCALL(glUniformMatrix4x2fv(location, count, transpose, value));
		}

		// Program

		static FORCEINLINE GLuint CreateProgram()
		{
			GLuint resource;
			GLCALL(resource = glCreateProgram());

			return resource;
		}

		static FORCEINLINE void UseProgram(GLuint Program)
		{
			GLCALL(glUseProgram(Program));
		}

		static FORCEINLINE void DeleteProgram(GLuint Program)
		{
			GLCALL(glDeleteProgram(Program));
		}

		static FORCEINLINE void ValidateProgram(GLuint Program)
		{
			GLCALL(glValidateProgram(Program));
		}

		static FORCEINLINE void AttachShader(GLuint Program, GLuint Shader)
		{
			GLCALL(glAttachShader(Program, Shader));
		}

		static FORCEINLINE void LinkProgram(GLuint Program)
		{
			GLCALL(glLinkProgram(Program));
		}

		static FORCEINLINE void GetProgramiv(GLuint Program, GLenum ParameterName, GLint* Value)
		{
			GLCALL(glGetProgramiv(Program, ParameterName, Value));
		}

		static FORCEINLINE void GetProgramInfoLog(
													GLuint Program,
													GLsizei BufferSize,
													GLsizei* Length,
													GLchar* InfoLogBuffer)
		{
			GLCALL(glGetProgramInfoLog(Program, BufferSize, Length, InfoLogBuffer));
		}

		static FORCEINLINE GLuint CreateShaderProgram(GLenum Type, GLsizei Count, const GLchar* const *Sources)
		{
			GLuint program;
			GLCALL(program = glCreateShaderProgramv(Type, Count, Sources));
		
			return program;
		}

		// VAO

		static FORCEINLINE void GenVertexArrays(GLsizei Count, GLuint* Arrays)
		{
			GLCALL(glGenVertexArrays(Count, Arrays));
		}

		static FORCEINLINE void DeleteVertexArrays(GLsizei Count, const GLuint* Arrays)
		{
			GLCALL(glDeleteVertexArrays(Count, Arrays));
		}

		static FORCEINLINE void BindVertexArray(GLuint Array)
		{
			GLCALL(glBindVertexArray(Array));
		}

		static FORCEINLINE void EnableVertexAttribArray(GLuint Index)
		{
			GLCALL(glEnableVertexAttribArray(Index));
		}

		static FORCEINLINE void DisableVertexAttribArray(GLuint Index)
		{
			GLCALL(glDisableVertexAttribArray(Index));
		}

		static FORCEINLINE void VertexAttribPointer(
													GLuint Index,
													GLint Size,
													GLenum Type,
													GLboolean Normalized,
													GLsizei Stride,
													CMemPtr Offset)
		{
			GLCALL(glVertexAttribPointer(Index, Size, Type, Normalized, Stride, Offset));
		}
		
		static FORCEINLINE void VertexAttribIPointer(
													GLuint Index,
													GLint Size,
													GLenum Type,
													GLsizei Stride,
													CMemPtr Offset)
		{
			GLCALL(glVertexAttribIPointer(Index, Size, Type, Stride, Offset));
		}

		static FORCEINLINE void VertexAttribLPointer(
													GLuint Index,
													GLint Size,
													GLenum Type,
													GLsizei Stride,
													CMemPtr Offset)
		{
			GLCALL(glVertexAttribLPointer(Index, Size, Type, Stride, Offset));
		}

		// #TODO not sure about this
		static FORCEINLINE void VertexAttribFormat(
													GLuint attribIndex,
													GLint size,
													GLenum type,
													GLboolean normalized,
													GLuint relativeOffset)
		{
			GLCALL(glVertexAttribFormat(attribIndex, size, type, normalized, relativeOffset));
		}


		// Textures

		static FORCEINLINE void GenTextures(GLsizei n, GLuint* textures)
		{
			GLCALL(glGenTextures(n, textures));
		}

		static FORCEINLINE void BindTexture(GLenum Target, GLuint Texture)
		{
			GLCALL(glBindTexture(Target, Texture));
		}

		static FORCEINLINE void ActiveTexture(GLenum Texture)
		{
			GLCALL(glActiveTexture(Texture));
		}

		static FORCEINLINE void GenerateMipmap(GLenum Target)
		{
			GLCALL(glGenerateMipmap(Target));
		}

		static FORCEINLINE void GenerateTextureMipmap(GLuint Texture)
		{
			GLCALL(glGenerateTextureMipmap(Texture));
		}

		static FORCEINLINE void DeleteTextures(GLsizei n, const GLuint* textures)
		{
			GLCALL(glDeleteTextures(n, textures));
		}


		// Drawing

		static FORCEINLINE void DrawArrays( GLenum Mode, GLint First, GLsizei Count )
		{
			GLCALL( glDrawArrays( Mode, First, Count ) );
		}

		static FORCEINLINE void DrawElements( GLenum Mode, GLsizei Count, GLenum Type, CMemPtr Indices )
		{
			GLCALL( glDrawElements( Mode, Count, Type, Indices ) );
		}

		//static FORCEINLINE void ObjectLabel(GLenum Identifier​, GLuint Name​, GLsizei Length​, const GLchar* Label ​)
		//{
		//	/*GLCALL*/(glObjectLabel(Identifier​, Name​, Length​, Label));
		//}

		static FORCEINLINE void ObjectLabel(GLenum Identifier, GLuint Name, GLsizei Length, const GLchar* Label)
		{
			glObjectLabel(Identifier, Name, Length, Label);
		}

		static FORCEINLINE void ObjectPtrLabel(MemPtr Ptr, GLsizei Length, const GLchar* Label)
		{
			glObjectPtrLabel(Ptr, Length, Label);
		}

		//static FORCEINLINE void ObjectPtrLabel(MemPtr Ptr​, GLsizei Length​, const GLchar* Label​)
		//{
		//	/*GLCALL*/(glObjectPtrLabel(Ptr​, Length​, Label​));
		//}

		// Other
	};

}
