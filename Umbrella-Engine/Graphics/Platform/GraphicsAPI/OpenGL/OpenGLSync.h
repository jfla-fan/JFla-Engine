#pragma once
#include "../GpuApi.h"
#include "OpenGLContext.h"



namespace UE::Graphics
{

	//struct OpenGLSync final : public SGpuFence
	//{
	//	GLsync Sync;
	//	
	//	OpenGLSync();
	//	~OpenGLSync();

	//	// GPU Fence Interface

	//	void Lock() override;
	//	bool Lock(Utils::TimeSpan timeout) override;

	//	void Unlock() override;
	//	bool IsLocked() const override;

	//	// for the moment, but does no lock
	//	bool WaitForLock() override;
	//	bool WaitForLock(Utils::TimeSpan timeout) override;
	//	

	//};

}

