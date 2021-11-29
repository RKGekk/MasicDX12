#include "dx12_utility.h"

#include <fstream>

#include <d3dcompiler.h>
#include <directx/d3dx12.h>

#include "com_exception.h"
#include "string_utility.h"

using namespace std::literals;

Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::wstring& filename) {
	std::ifstream fin(filename, std::ios::binary);

	fin.seekg(0, std::ios_base::end);
	std::ifstream::pos_type size = (int)fin.tellg();
	fin.seekg(0, std::ios_base::beg);

	Microsoft::WRL::ComPtr<ID3DBlob> blob;
	HRESULT hr = D3DCreateBlob(size, blob.GetAddressOf());
	COM_ERROR_IF_FAILED(hr, "Failed to create d3d blob.");

	fin.read((char*)blob->GetBufferPointer(), size);
	fin.close();

	return blob;
}

Microsoft::WRL::ComPtr<ID3DBlob> LoadBinary(const std::string& filename) {
	return LoadBinary(s2w(filename));
}

Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmd_list, const void* init_data, UINT64 byte_size, Microsoft::WRL::ComPtr<ID3D12Resource>& upload_buffer) {
	HRESULT hr;
	Microsoft::WRL::ComPtr<ID3D12Resource> default_buffer;

	// Create the actual default buffer resource.
	D3D12_HEAP_PROPERTIES heap_properties_default = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	D3D12_RESOURCE_DESC resource_desc = CD3DX12_RESOURCE_DESC::Buffer(byte_size);
	hr = device->CreateCommittedResource(&heap_properties_default, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(default_buffer.GetAddressOf()));
	COM_ERROR_IF_FAILED(hr, "Failed to create default committed resource.");

	// In order to copy CPU memory data into our default buffer, we need to create
	// an intermediate upload heap.
	D3D12_HEAP_PROPERTIES heap_properties_upload = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	hr = device->CreateCommittedResource(&heap_properties_upload, D3D12_HEAP_FLAG_NONE, &resource_desc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(upload_buffer.GetAddressOf()));

	// Describe the data we want to copy into the default buffer.
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = init_data;
	subResourceData.RowPitch = byte_size;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	// Schedule to copy the data to the default buffer resource. At a high level, the helper function UpdateSubresources
	// will copy the CPU memory into the intermediate upload heap. Then, using ID3D12CommandList::CopySubresourceRegion,
	// the intermediate upload heap data will be copied to mBuffer.
	D3D12_RESOURCE_BARRIER resource_barrier_common_to_copy_dest = CD3DX12_RESOURCE_BARRIER::Transition(default_buffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST);
	cmd_list->ResourceBarrier(1, &resource_barrier_common_to_copy_dest);
	UpdateSubresources<1>(cmd_list, default_buffer.Get(), upload_buffer.Get(), 0, 0, 1, &subResourceData);

	D3D12_RESOURCE_BARRIER resource_barrier_copy_dest_to_generic_read = CD3DX12_RESOURCE_BARRIER::Transition(default_buffer.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ);
	cmd_list->ResourceBarrier(1, &resource_barrier_copy_dest_to_generic_read);

	// Note: uploadBuffer has to be kept alive after the above function calls because
	// the command list has not been executed yet that performs the actual copy.
	// The caller can Release the uploadBuffer after it knows the copy has been executed.
	return default_buffer;
}

Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* initData, UINT64 byteSize) {
	Microsoft::WRL::ComPtr<ID3D12Resource> upload_buffer;
	return CreateDefaultBuffer(device, cmdList, initData, byteSize, upload_buffer);
}

Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::wstring& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target) {
	UINT compileFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	Microsoft::WRL::ComPtr<ID3DBlob> byteCode = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errors;
	HRESULT hr = D3DCompileFromFile(filename.c_str(), defines, D3D_COMPILE_STANDARD_FILE_INCLUDE, entrypoint.c_str(), target.c_str(), compileFlags, 0, &byteCode, &errors);

	if (errors != nullptr) OutputDebugStringA((char*)errors->GetBufferPointer());

	COM_ERROR_IF_FAILED(hr, "Failed to compile shader.");

	return byteCode;
}

Microsoft::WRL::ComPtr<ID3DBlob> CompileShader(const std::string& filename, const D3D_SHADER_MACRO* defines, const std::string& entrypoint, const std::string& target) {
	return CompileShader(s2w(filename), defines, entrypoint, target);
}
