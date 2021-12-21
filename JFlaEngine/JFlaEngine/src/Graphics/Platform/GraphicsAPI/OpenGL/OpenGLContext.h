#pragma once
#include "../../../../Core.h"
#include "glad/glad.h"


namespace Engine::Graphics
{

	struct OpenGLContext
	{
		constexpr static GLuint GInvalidGLResource = 0;

		static FORCEINLINE void ClearBufferfv(GLenum Buffer, GLint DrawBufferIndex, const GLfloat* value)
		{
			glClearBufferfv(Buffer, DrawBufferIndex, value);
		}

		static FORCEINLINE void ClearBufferfi(GLenum Buffer, GLint DrawBufferIndex, GLfloat Depth, GLint Stencil)
		{
			glClearBufferfi(Buffer, DrawBufferIndex, Depth, Stencil);
		}

		static FORCEINLINE void ClearBufferiv(GLenum Buffer, GLint DrawBufferIndex, const GLint* Value)
		{
			glClearBufferiv(Buffer, DrawBufferIndex, Value);
		}

		static FORCEINLINE void ClearDepth(GLdouble Depth)
		{
			glClearDepth(Depth);
		}

		static FORCEINLINE void ClearColor(GLfloat Red, GLfloat Green, GLfloat Blue, GLfloat Alpha)
		{
			glClearColor(Red, Green, Blue, Alpha);
		}

		static FORCEINLINE void Clear(GLbitfield Mask)
		{
			glClear(Mask);
		}
		
		static FORCEINLINE void PointSize(GLfloat Size)
		{
			check(Size >= 1.f);
			glPointSize(Size);
		}

		// Buffers

		static FORCEINLINE void GenBuffers(GLsizei n, GLuint* buffers)
		{
			glGenBuffers(n, buffers);
		}

		static FORCEINLINE void CreateBuffers(GLsizei n, GLuint* buffers)
		{
			glCreateBuffers(n, buffers);
		}

		static FORCEINLINE void DeleteBuffers(GLsizei n, GLuint* buffers)
		{
			glDeleteBuffers(n, buffers);
		}

		static FORCEINLINE void BindBuffer(GLenum Target, GLuint Buffer)
		{
			glBindBuffer(Target, Buffer);
		}

		static FORCEINLINE void BufferData(GLenum Target, GLsizeiptr Size, CMemPtr Data, GLenum Usage)
		{
			glBufferData(Target, Size, Data, Usage);
		}

		static FORCEINLINE void NamedBufferData(GLuint Buffer, GLsizeiptr Size, CMemPtr Data, GLenum Usage)
		{
			glNamedBufferData(Buffer, Size, Data, Usage);
		}

		static FORCEINLINE void CopyBufferSubData(GLenum ReadTarget, GLenum WriteTarget, GLintptr ReadOffset, GLintptr WriteOffset, GLsizeiptr Size)
		{
			glCopyBufferSubData(ReadTarget, WriteTarget, ReadOffset, WriteOffset, Size);
		}

		static FORCEINLINE void CopyNamedBufferSubData(GLuint ReadBuffer, GLuint WriteBuffer, GLintptr ReadOffset, GLintptr WriteOffset, GLsizeiptr Size)
		{
			glCopyNamedBufferSubData(ReadBuffer, WriteBuffer, ReadOffset, WriteOffset, Size);
		}

		// Shaders

		static FORCEINLINE GLuint CreateShader(GLenum Type)
		{
			return glCreateShader(Type);
		}

		static FORCEINLINE void ShaderSource(GLuint Shader, GLsizei Count, const GLchar* const *String, const GLint* Length)
		{
			glShaderSource(Shader, Count, String, Length);
		}

		static FORCEINLINE void CompileShader(GLuint Shader)
		{
			glCompileShader(Shader);
		}

		static FORCEINLINE void GetShaderiv(GLuint Shader, GLenum ParameterName, GLint* Value)
		{
			glGetShaderiv(Shader, ParameterName, Value);
		}

		static FORCEINLINE void GetShaderInfoLog(GLuint Shader, GLsizei BufferSize, GLsizei* Length, GLchar* InfoLogBuffer)
		{
			glGetShaderInfoLog(Shader, BufferSize, Length, InfoLogBuffer);
		}

		static FORCEINLINE void DeleteShader(GLuint Shader)
		{
			glDeleteShader(Shader);
		}

		static FORCEINLINE GLuint CreateProgram()
		{
			return glCreateProgram();
		}

		static FORCEINLINE void UseProgram(GLuint Program)
		{
			glUseProgram(Program);
		}

		static FORCEINLINE void AttachShader(GLuint Program, GLuint Shader)
		{
			glAttachShader(Program, Shader);
		}

		static FORCEINLINE void LinkProgram(GLuint Program)
		{
			glLinkProgram(Program);
		}

		// Drawing

		static FORCEINLINE void DrawArrays(GLenum Mode, GLint First, GLsizei Count)
		{
			glDrawArrays(Mode, First, Count);
		}

		static FORCEINLINE void DrawElements(GLenum Mode, GLsizei Count, GLenum Type, CMemPtr Indices)
		{
			glDrawElements(Mode, Count, Type, Indices);
		}
	};

}
