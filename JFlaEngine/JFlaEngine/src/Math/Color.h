#pragma once
#include "../Core.h"



namespace Engine::Math
{
	class LinearColor;
	class Color;

	class Color
	{
	private:
		
		union
		{
			uint8 channels[4];

			// red component
			uint8 r;
		
			// green component
			uint8 g;

			// blue component
			uint8 b;

			// alpha (opacity) component
			uint8 a;
	
		} color;


	public:

		explicit Color()
		{
			Memory::Memcpy(White.GetValue(), this->color.channels, sizeof(this->color.channels));
		}

		Color(uint8 InRed, uint8 InGreen, uint8 InBlue, uint8 InAlpha = 255)
		{
			color.r = InRed;
			color.g = InGreen;
			color.b = InBlue;
			color.a = InAlpha;
		}

		~Color() = default;
		
		uint8 R() const { return color.r; }
		
		uint8 G() const { return color.g; }
		
		uint8 B() const { return color.b; }
		
		uint8 A() const { return color.a; }

		const uint8* GetValue() const { return color.channels; }

		static LinearColor ToLinearColor(const Color& InColor);


		static const Color White;
		static const Color Black;
		static const Color Transparent;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Cyan;
		static const Color Magenta;
		static const Color Orange;
		static const Color Purple;
		static const Color Turquoise;
		static const Color Silver;
		static const Color Emerald;

	};

	class LinearColor
	{
	private:

		union {

			float	channels[4];

			float	r,
					g,
					b,
					a;

		} color;

	public:

		explicit LinearColor()
		{
			color.r = 0.f;
			color.g = 0.f;
			color.b = 0.f;
			color.a = 1.f;
		}

		LinearColor(float InR, float InG, float InB, float InA = 1.0f)
		{
			color.r = InR;
			color.g = InG;
			color.b = InB;
			color.a = InA;
		}

		float R() const { return color.r; }

		float G() const { return color.g; }

		float B() const { return color.b; }

		float A() const { return color.a; }

		
		const float* GetValue() const { return color.channels; }

		static Color ToColor(const LinearColor& InColor);

		
		static const LinearColor White;
		static const LinearColor Gray;
		static const LinearColor Black;
		static const LinearColor Transparent;
		static const LinearColor Red;
		static const LinearColor Green;
		static const LinearColor Blue;
		static const LinearColor Yellow;

	};

}


