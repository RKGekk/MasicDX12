#include "engine_options.h"

#include <algorithm>

#include <DirectXMath.h>
#include <pugixml/pugixml.hpp>

using namespace std::literals;

EngineOptions::EngineOptions() {}

EngineOptions::EngineOptions(const std::string& xmlFilePath) : EngineOptions() {
	Init(xmlFilePath);
}

EngineOptions::~EngineOptions() {}

bool EngineOptions::Init(const std::string& xml_file_name) {
	pugi::xml_document xml_doc;
	pugi::xml_parse_result parse_res = xml_doc.load_file(xml_file_name.c_str());
	if (!parse_res) { return false;	}

	pugi::xml_node root_node = xml_doc.child("PlayerOptions");
	if (!root_node) { return false; }

	pugi::xml_node graphics_node = root_node.child("Graphics");
	if (graphics_node) {
		pugi::xml_node debug_screen_node = graphics_node.child("DebugUI");
		if (debug_screen_node) {
			std::string sDebug(debug_screen_node.first_child().value());
			std::for_each(sDebug.begin(), sDebug.end(), [](char& c) { c = ::toupper(c); });
			m_debug_ui = (sDebug == "YES" || sDebug == "TRUE" || sDebug == "1") ? true : false;
		}

		pugi::xml_node full_screen_max_node = graphics_node.child("FullScreenMax");
		if (full_screen_max_node) {
			std::string sFullScreenMax(full_screen_max_node.first_child().value());
			std::for_each(sFullScreenMax.begin(), sFullScreenMax.end(), [](char& c) { c = ::toupper(c); });
			m_fullScreenMax = (sFullScreenMax == "YES" || sFullScreenMax == "TRUE" || sFullScreenMax == "1") ? true : false;
		}

		pugi::xml_node full_screen_node = graphics_node.child("FullScreen");
		if (full_screen_node) {
			std::string sFullScreen(full_screen_node.first_child().value());
			std::for_each(sFullScreen.begin(), sFullScreen.end(), [](char& c) { c = ::toupper(c); });
			m_fullScreen = (sFullScreen == "YES" || sFullScreen == "TRUE" || sFullScreen == "1") ? true : false;
		}

		pugi::xml_node vsync_node = graphics_node.child("RunFullSpeed");
		if (vsync_node) {
			std::string sVSync(vsync_node.first_child().value());
			std::for_each(sVSync.begin(), sVSync.end(), [](char& c) { c = ::toupper(c); });
			m_runFullSpeed = (sVSync == "YES" || sVSync == "TRUE" || sVSync == "1") ? true : false;
		}

		pugi::xml_node height_node = graphics_node.child("Height");
		if (height_node) {
			std::string sHeight(height_node.first_child().value());
			m_screenHeight = std::stoi(sHeight);
		}

		pugi::xml_node width_node = graphics_node.child("Width");
		if (width_node) {
			std::string sWidth(width_node.first_child().value());
			m_screenWidth = std::stoi(sWidth);
		}

		pugi::xml_node renderer_node = graphics_node.child("Renderer");
		if (renderer_node) {
			std::string sRenderer(renderer_node.first_child().value());
			if (sRenderer == "Direct3D 11"s) m_Renderer = Renderer::Renderer_D3D11;
			else if (sRenderer == "Direct3D 12"s) m_Renderer = Renderer::Renderer_D3D12;
			else if (sRenderer == "Direct3D 9"s) m_Renderer = Renderer::Renderer_D3D9;
			else if (sRenderer == "OpenGL"s) m_Renderer = Renderer::Renderer_OpenGL;
			else if (sRenderer == "Vulkan"s) m_Renderer = Renderer::Renderer_Vulkan;
		}
	}


	pugi::xml_node audio_node = root_node.child("Graphics");

	
		
	
	return true;
}