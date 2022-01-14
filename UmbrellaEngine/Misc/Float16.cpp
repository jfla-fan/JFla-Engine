#include "Float16.h"


namespace J
{
	Float16::Float16(float InValue)
	{
		// #todo: perform necessary checks and set;
		this->Value = (float)InValue;
	}

	Float16::Float16(const Float16& another)
	{
		this->Value = another.Value;
	}

	Float16::Float16(Float16&& another)
	{
		this->Value = std::move(another.Value);
		another.Value = 0;
	}

	inline Float16& Float16::operator=(const Float16& another)
	{
		if (this != &another)
		{
			this->Value = another.Value;
		}

		return *this;
	}

	inline Float16& Float16::operator=(Float16&& another)
	{
		this->Value = another.Value;
		another.Value = 0;

		return *this;
	}

	inline Float16::operator float() const
	{
		// #todo Make it work
	}
}
