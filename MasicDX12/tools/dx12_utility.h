#pragma once

#include <string>

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>

UINT CalcConstantBufferByteSize(UINT byteSize) {
	// Constant buffers must be a multiple of the minimum hardware
	// allocation size (usually 256 bytes).  So round up to nearest
	// multiple of 256.  We do this by adding 255 and then masking off
	// the lower 2 bytes which store all bits < 256.
	// Example: Suppose byteSize = 300.
	// (300 + 255) & ~255
	// 555 & ~255
	// 0x022B & ~0x00ff
	// 0x022B & 0xff00
	// 0x0200
	// 512
	return (byteSize + 255) & ~255;
}

Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::wstring& filename);
Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::string& filename);

Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmd_list, const void* init_data, UINT64 byte_size, Microsoft::WRL::ComPtr<ID3D12Resource>& upload_buffer);
Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmd_list, const void* init_data, UINT64 byte_size);

static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);
static Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::string& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target);