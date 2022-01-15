#pragma once
#include "../Texture.h"
#include "../../../../Math/Math.h"
#include "OpenGLContext.h"



namespace J::Utils
{
	class Image;

	using ImagePtr = Ref<Image>;
}

namespace J::Graphics
{
	using namespace Math;
	using namespace GpuApi;


	using Utils::Image;
	using Utils::ImagePtr;



	
	/**
	 * 2D Texture Class.
	 */
	class OpenGLTexture final : public JTexture
	{
	public:

		using IdType = uint32;

	public:

		/**
		 * #todo IMPLEMENT MEMORY POOLS IN FUTURE TO USE THEM LIKE THIS: new (size_t, MemoryPool& )
		 * 
		 */

	private:

		IdType Resource;
		
		mutable bool bLoaded;					// is data loaded on the GPU
		
		mutable bool bInitialized;				// is resource valid and meta data properly initialized
		
		mutable bool bHasMipChain;				// Does this texture have mip maps?
		
		STextureSizeInfo SizeInfo;
		
		ETextureFormat Format;

		system::FilePath ResourcePath;

		
		// OpenGL Specific, get rid of this some day

		mutable IdType Active = 0;	// Slot

		GLenum TextureType = GL_TEXTURE_2D;

	private:

		NODISCARD bool AllocateResource();	// GPU API ?

	public:

		OpenGLTexture();
		~OpenGLTexture();

		OpenGLTexture(const system::FilePath& path, bool genMipChain = false);

		OpenGLTexture(const OpenGLTexture& another) = delete;
		OpenGLTexture& operator = (const OpenGLTexture& another) = delete;

		OpenGLTexture(OpenGLTexture&& another) NOEXCEPT;
		OpenGLTexture& operator = (OpenGLTexture&& another) NOEXCEPT;

		void Release() const;


		void Bind() const;
		void Bind(uint32 id) const;
		void Unbind() const;


		INLINE IdType GetResource() const { return Resource; }
		
		INLINE IdType GetActiveId() const { return Active; }

		INLINE bool IsValid() const override { return bInitialized; }


		bool Load(const system::FilePath& path, ETextureFormat format = ETextureFormat::AUTO);

		bool Load(CMemPtr data, uint32 width, uint32 height, uint32 channels, ETextureFormat format = ETextureFormat::AUTO);
	
		bool Load(const Image& image, ETextureFormat format = ETextureFormat::AUTO);



		// #TODO in future TRenderPtr CreateResource, GetResource, ReleaseResource, GetImage [DONE], etc ...


		void					SetMinLOD(float lod);
		void					SetMaxLOD(float lod);
		void					SetBiasLOD(float bias);
		void					SetBorderColor(Vector4 color);
		void					SetWrappingMode(ETextureWrappingMode mode, ETextureWrappingAxis axices);

		float					GetMaxLOD() const;
		ImagePtr				GetImage() const;
		void					GenerateMipmapChain() const;
		bool					IsLoaded() const;
		bool					IsInitialized() const;
		bool					IsMultisampled() const;
		bool					IsFloatingPoint() const;
		bool					IsDepthOnly() const;
		Vector4					GetBorderColor() const;
		uint32					GetPixelSize() const;
		ETextureWrappingMode	GetWrapMode(ETextureWrappingAxis axis = ETextureWrappingAxis::S) const;	// assumes only 1 axis - S, T or R
		STextureSizeInfo		GetSizeInfo() const;
		system::FilePath		GetFilePath() const;
		float					GetBiasLOD() const;

	};


}


