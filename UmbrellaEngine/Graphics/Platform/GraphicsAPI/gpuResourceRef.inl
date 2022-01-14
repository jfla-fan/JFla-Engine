/////////////////////////////////////////////////////////////////////////////////////////
/// Copyright (c)
/// /// /////////////////////////////////////////////////////////////////////////////////////


#ifndef RES_TYPE
#error RES_TYPE is not defined. This file just builds struct of GpuApi resource Ref
#endif


#ifndef RESOURCE_REF_INL
#define RESOURCE_REF_INL


//struct ResourceRef
//{
//	uint32 Id;
//
//	constexpr FORCEINLINE uint32 ID() { return Id; }
//	constexpr FORCEINLINE bool IsNull() const { return Id == 0; }
//	constexpr FORCEINLINE explicit operator bool() { return !IsNull(); }
//
//	constexpr FORCEINLINE ResourceRef Null() { return ResourceRef( 0 ); }
//
//	explicit operator int() = delete;
//	explicit operator unsigned int() = delete;
//};



struct RES_TYPE
{
public:
	
	FORCEINLINE bool operator == ( const RES_TYPE& another ) { return Id == another.Id; }
	FORCEINLINE bool operator != ( const RES_TYPE& another ) { return Id != another.Id; }

public:

	constexpr FORCEINLINE uint32 ID() const { return Id; }
	constexpr FORCEINLINE bool IsNull() const { return Id == 0; }
	constexpr FORCEINLINE explicit operator bool() { return !IsNull(); }

	constexpr static FORCEINLINE RES_TYPE Null() { return RES_TYPE( 0 ); }

	explicit operator int() = delete;
	explicit operator unsigned int() = delete;
	
	
	uint32 Id;
};


#undef RESOURCE_REF_INL
#endif // RESOURCE_REF_INL


#ifndef	RESOURCEREF_NOREFCOUNT
#define RESOURCEREF_NOREFCOUNT

void AddRef( const RES_TYPE& );

int32 Release( const RES_TYPE& );


INLINE void AddRefIfValid( const RES_TYPE& res )
{
	if ( !res.IsNull() )
	{
		AddRef( res );
	}
}

INLINE int32 SafeRelease( RES_TYPE& res )
{
	int32 newRefCount = 0;
	if ( !res.IsNull() )
	{
		newRefCount = Release( res );
		res = RES_TYPE::Null();
	}

	return newRefCount;
}

INLINE void SafeChangeRef( RES_TYPE& res, bool inc )
{
	if ( !res.IsNull() )
	{
		if ( inc )
		{
			AddRef( res );
		}
		else
		{
			Release( res );
			res = RES_TYPE::Null();
		}
	}
}

#endif		// RESOURCEREF_NOREFCOUNT

#undef RES_TYPE
#undef RESOURCEREF_NOREFCOUNT