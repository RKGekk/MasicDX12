#pragma once

#include <string>

#include <DirectXMath.h>

#include "renderer_enum.h"

struct EngineOptions {
	Renderer m_Renderer = Renderer::Renderer_D3D12;
	bool m_runFullSpeed = false;
	bool m_fullScreen = false;
	bool m_fullScreenMax = true;
	int m_screenWidth = 800;
	int m_screenHeight = 600;
	bool m_debug_ui = true;

	float m_soundEffectsVolume = 1.0f;
	float m_musicVolume = 1.0f;

	EngineOptions();
	EngineOptions(const std::string& xmlFilePath);
	~EngineOptions();

	bool Init(const std::string& xmlFilePath);
};
