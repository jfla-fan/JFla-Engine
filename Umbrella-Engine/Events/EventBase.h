#pragma once
#include "../Core.h"
#include <string>


namespace UE
{

	enum class EEventCategory : uint8
	{
		Common		= 0,
		Application = BIT(0),
		Input		= Application << 1,
		Mouse		= Input << 1,
		MouseButton = Mouse << 1,
		Keyboard	= MouseButton << 1,
	};

	DECLARE_ENUM_FLAG_OPERATIONS( EEventCategory )

	class EventBase
	{
	public:
		~EventBase() = default;


		virtual uint8 GetEventCategoryFlags() const = 0;

		virtual uint32 GetEventId() const = 0;
		
		virtual const CHAR* GetName() const = 0;
		
		virtual std::string ToString() const = 0;

		bool InCategory(EEventCategory category) const { return GetEventCategoryFlags() & category; }
	};

#define EVENT_BODY(event_name)\
		uint32 GetEventId() const override { return std::hash()}
}