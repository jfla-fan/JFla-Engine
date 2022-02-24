#include "Color.h"


namespace UE::Math
{
	static const float OneOver255 = 1.0f / 255.0f;


	const LinearColor LinearColor::White( 1.f, 1.f, 1.f );
	const LinearColor LinearColor::Gray( 0.5f, 0.5f, 0.5f );
	const LinearColor LinearColor::Black( 0, 0, 0 );
	const LinearColor LinearColor::Transparent( 0, 0, 0, 0 );
	const LinearColor LinearColor::Red( 1.f, 0, 0 );
	const LinearColor LinearColor::Green( 0, 1.f, 0 );
	const LinearColor LinearColor::Blue( 0, 0, 1.f );
	const LinearColor LinearColor::Yellow( 1.f, 1.f, 0 );


	const Color Color::White( ( uint8 )255, ( uint8 )255, ( uint8 )255 );
	const Color Color::Black( ( uint8 )0, ( uint8 )0, ( uint8 )0 );
	const Color Color::Transparent( ( uint8 )0, ( uint8 )0, ( uint8 )0, ( uint8 )0 );
	const Color Color::Red( ( uint8 )255, ( uint8 )0, ( uint8 )0 );
	const Color Color::Green( ( uint8 )0, ( uint8 )255, ( uint8 )0 );
	const Color Color::Blue( ( uint8 )0, ( uint8 )0, ( uint8 )255 );
	const Color Color::Yellow( ( uint8 )255, ( uint8 )255, ( uint8 )0 );
	const Color Color::Cyan( ( uint8 )0, ( uint8 )255, ( uint8 )255 );
	const Color Color::Magenta( ( uint8 )255, ( uint8 )0, ( uint8 )255 );
	const Color Color::Orange( ( uint8 )243, ( uint8 )156, ( uint8 )18 );
	const Color Color::Purple( ( uint8 )169, ( uint8 )7, ( uint8 )228 );
	const Color Color::Turquoise( ( uint8 )26, ( uint8 )188, ( uint8 )156 );
	const Color Color::Silver( ( uint8 )189, ( uint8 )195, ( uint8 )199 );
	const Color Color::Emerald( ( uint8 )46, ( uint8 )204, ( uint8 )113 );


	Color LinearColor::ToColor( const LinearColor& InColor )
	{
		return Color
					(
						static_cast< uint8 >( InColor.R() * MAX_UINT8 ),
						static_cast< uint8 >( InColor.B() * MAX_UINT8 ),
						static_cast< uint8 >( InColor.G() * MAX_UINT8 ),
						static_cast< uint8 >( InColor.A() * MAX_UINT8 )
					);
	}

	LinearColor Color::ToLinearColor( const Color& InColor )
	{
		return LinearColor
					(
						OneOver255 * InColor.R(), OneOver255 * InColor.G(),
						OneOver255 * InColor.B(), OneOver255 * InColor.A()
					);
	}

}