#pragma once
#include "../GpuApi.h"
#include "OpenGLVertexBuffer.h"
#include "OpenGLIndexBuffer.h"
#include "../../../../Math/Math.h"


namespace J::Graphics
{
	using namespace J::Math;


	class OpenGLVertexArray;

	// vertex attributes


	/**
	*							------ Entries Count --------
	*	 ------- Elements count ----------	  ----------- Elements count ------------
	* { [element1, element2, ... elementN], [element1, element2, ... elementN], ... }
	*
	*/
	struct SVertexAttribute
	{
		GLenum	Type;

		bool	bNormalized;

		bool	bDisable;

		uint8	ElementsCount;

		uint32	EntriesCount;

		SIZE_T	SizeBytes;

		template<class _Ty>
		static SVertexAttribute GetAttribute(bool disable = false);

		// ?? independence of Type
		static void SetVertexAttribute(Ref<OpenGLVertexArray> array, SVertexAttribute attribute);
	};



	// Vertex Array

	class OpenGLVertexArray
	{
	public:

		using IdType = uint32;


	public:

		OpenGLVertexArray();
		~OpenGLVertexArray();
		
		OpenGLVertexArray(const OpenGLVertexArray& another) = delete;
		OpenGLVertexArray& operator = (const OpenGLVertexArray& another) = delete;

		OpenGLVertexArray(OpenGLVertexArray&& another) NOEXCEPT;
		OpenGLVertexArray& operator = (OpenGLVertexArray&& another) NOEXCEPT;

		void Bind() const;
		void Unbind() const;

		IdType GetHandle() const { return Resource; }
		IdType GetCurrentlyBoundedVertexArray() const { return CurrentlyBoundVertexArray; }
		IdType GetNextAttributeIndex() const { return AttributeIndex; }

		//const std::vector<Ref<OpenGLVertexBuffer>>& GetVertexBuffers() const NOEXCEPT { return VertexBuffers; }
		Ref<OpenGLIndexBuffer> GetIndexBuffer() const NOEXCEPT { return IndexBuffer; }


		template<std::ranges::forward_range _Range>
			requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, SVertexAttribute> &&
					std::ranges::sized_range<_Range>
		void AddVertexLayout(Ref<OpenGLVertexBuffer> buffer, _Range&& layout);

		template<std::ranges::forward_range _Range>
			requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, SVertexAttribute>
		void RemoveVertexLayout(_Range&& layout);

		void SetIndexBuffer(Ref<OpenGLIndexBuffer> indexBuffer);

	protected:

		static constexpr IdType InvalidVertexArrayId = 0;	// OPENGL CORE PROFILE. It is not invalid if OPENGL COMPATIBILITY PROFILE IS ENABLED
		void LinkIndexBuffer(Ref<OpenGLIndexBuffer> indexBuffer);

		void Release();

	private:

		IdType Resource;
		uint32 AttributeIndex;

		static IdType CurrentlyBoundVertexArray;

		/*
		* possibly implement buffer -> List<Layout> layouts mapping
		*/

		Ref<OpenGLIndexBuffer> IndexBuffer;

	};


	template<std::ranges::forward_range _Range>
		requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, SVertexAttribute> &&
				std::ranges::sized_range<_Range>
	void OpenGLVertexArray::AddVertexLayout(Ref<OpenGLVertexBuffer> buffer, _Range&& layout)
	{
		check(buffer->IsValid());

		this->Bind();

		buffer->Bind();

		SIZE_T offset = 0;
		int32 stride = 0;

		for (const auto& attribute : layout)
		{
			stride += static_cast<int32>(attribute.SizeBytes);
		}


		for (const auto& attribute : layout)
		{
			for (uint32 i = 0; i < attribute.EntriesCount; ++i)
			{
				// handle other types with Normalized = GL_TRUE (UINT types), EnableVertexAttribIPointer, etc

				if (!attribute.bDisable)
				{
					OpenGLContext::EnableVertexAttribArray(this->AttributeIndex);
				}
				
				OpenGLContext::VertexAttribPointer(
									this->AttributeIndex,
									attribute.ElementsCount,
									attribute.Type,
									GL_FALSE,
									stride,
									reinterpret_cast<CMemPtr>(offset));

				offset += attribute.SizeBytes / attribute.EntriesCount;
				++this->AttributeIndex;
			}
		}

		//this->VertexBuffers.push_back(buffer);
	}

	template<std::ranges::forward_range _Range>
		requires std::is_same_v<typename std::iterator_traits<std::ranges::iterator_t<_Range>>::value_type, SVertexAttribute>
	void OpenGLVertexArray::RemoveVertexLayout(_Range&& layout)
	{
		check(this->AttributeIndex >= layout.size());

		this->Bind();

		for (const auto& attribute : layout)
		{
			for (uint32 i = 0; i < attribute.EntriesCount; ++i)
			{
				if (attribute.bDisable)
				{
					OpenGLContext::DisableVertexAttribArray(--this->AttributeIndex);
				}
			}
		}

		this->Unbind();
	}
}
