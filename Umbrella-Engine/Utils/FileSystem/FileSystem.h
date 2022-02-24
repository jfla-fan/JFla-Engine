#pragma once

#include "../../Core.h"
#include <filesystem>
#include <fstream>



namespace UE::system
{
	using FilePath		= std::filesystem::path;
	using FileStream	= std::fstream;


	class File
	{
	public:

		enum class EMode : int32
		{
			READ = std::ios::in,
			WRITE = std::ios::out,
			BINARY = std::ios::binary,
		};

	public:

		File( const FilePath& path, EMode mode = EMode::READ );

		File( std::string_view path, EMode mode = EMode::READ );

		File( const CHAR* path, EMode mode = EMode::READ );

		File( const File& another ) = delete;
		
		File& operator = ( const File& another ) = delete;

		File( File&& another ) NOEXCEPT;
		
		File& operator = ( File&& another ) NOEXCEPT;

		~File();


		bool IsOpen() const;

		bool IsGood() const;

		void Open( const FilePath& path, EMode mode = EMode::READ );
		
		void Open( std::string_view path, EMode mode = EMode::READ );
		
		void Open( const CHAR* path, EMode mode = EMode::READ );

		void Close();

		FileStream& GetStream();

		FilePath GetPath() const;

		std::string ReadAllText();

		template< class _Ty >
		File& operator >> ( _Ty&& Value );

		template< class _Ty >
		File& operator << ( _Ty&& Value );

		void ReadBytes( MemPtr Data, SIZE_T Size );

		void WriteBytes( CMemPtr Data, SIZE_T Size );

	public:

		static bool Exists( const FilePath& path );

		static bool Exists( std::string_view path );

		static bool Exists( const CHAR* path );

		static bool IsFile( const FilePath& path );

		static bool IsFile( std::string_view path );

		static bool IsFile( const CHAR* path );

		static std::string ReadAllText( const FilePath& path );

		static std::string ReadAllText( std::string_view path );

		static std::string ReadAllText( const CHAR* path );


	private:

		void _open( const FilePath& path, EMode mode = EMode::READ );

		void _open( std::string_view path, EMode mode = EMode::READ );

		void _open( const CHAR* path, EMode mode = EMode::READ );

		FilePath	Path;

		FileStream	Stream;

		EMode		Mode;
	};

	DECLARE_ENUM_FLAG_OPERATIONS( File::EMode )



	template< class _Ty >
	File& File::operator << ( _Ty&& Value )
	{
		this->Stream << std::forward< _Ty >( Value );
		return *this;
	}

	template< class _Ty >
	File& File::operator >> ( _Ty&& Value )
	{
		this->Stream >> std::forward< _Ty >( Value );
		return *this;
	}


	bool IsDirectory( const FilePath& path );

	bool IsDirectory( std::string_view path );

	bool IsDirectory( const CHAR* path );


	bool CreateDirectory( const FilePath& path );

	bool CreateDirectory( std::string_view path );

	bool CreateDirectory( const CHAR* path );

}
