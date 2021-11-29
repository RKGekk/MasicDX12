#pragma once

#include <memory>
#include <vector>
#include <wrl.h>
#include <d3d12.h>

#include "../tools/dx12_utility.h"

template<class T>
class VertexBuffer {
public:
	VertexBuffer() {};

	VertexBuffer(const VertexBuffer<T>& rhv) {
		m_buffer = rhv.m_buffer;
		m_buffer_size = rhv.m_buffer_size;
		m_stride = rhv.m_stride;
	};

	VertexBuffer<T>& operator=(const VertexBuffer<T> rhv) {
		if (this == &rhv) { return *this; }
		m_buffer = rhv.m_buffer;
		m_buffer_size = rhv.m_buffer_size;
		m_stride = rhv.m_stride;
		return *this;
	}

	ID3D12Resource* Get() const {
		return m_buffer.Get();
	}

	ID3D12Resource* const* GetAddressOf() {
		return m_buffer.GetAddressOf();
	}

	UINT VertexCount() const {
		return m_buffer_size;
	}

	const D3D12_VERTEX_BUFFER_VIEW* BufferViewPtr() {
		return &m_buffer_view;
	}

	const D3D12_VERTEX_BUFFER_VIEW& BufferViewRef() {
		return m_buffer_view;
	}

	D3D12_VERTEX_BUFFER_VIEW BufferView() {
		return m_buffer_view;
	}

	UINT Stride() const {
		return m_stride;
	}

	const UINT* StridePtr() {
		return &m_stride;
	}

	HRESULT Initialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmd_list, const std::vector<T>& data) {
		if (m_buffer.Get() != nullptr) { m_buffer.Reset(); }

		m_buffer_size = (UINT)data.size();
		m_stride = sizeof(T);

		UINT sz = m_stride * m_buffer_size;
		m_buffer = CreateDefaultBuffer(device, cmd_list, data.data(), sz);
		m_buffer_view.BufferLocation = m_buffer->GetGPUVirtualAddress();
		m_buffer_view.SizeInBytes = m_stride;
		m_buffer_view.SizeInBytes = sz;

		return S_OK;
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer;
	UINT m_stride = 0;
	UINT m_buffer_size = 0;
	D3D12_VERTEX_BUFFER_VIEW m_buffer_view;
};