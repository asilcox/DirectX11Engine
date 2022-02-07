#pragma once
#include "Mesh.h"

class Cube : public Mesh
{
public:
	Cube(RenderManager& renderManager)
		:
		Mesh(renderManager)
	{
		transform.pos = { 0.0f, 0.0f, 5.0f };
		transform.rot = { 0.0f, 0.0f, 0.0f };
		transform.scale = { 1.0f, 1.0f, 1.0f };

		vertices =
		{
			{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 0.0f, 0.0f } }, // 0
			{ { 1.0f, -1.0f, -1.0f }, { 0.0f, 1.0f, 0.0f } }, // 1
			{ { -1.0f, 1.0f, -1.0f }, { 0.0f, 0.0f, 1.0f } }, // 2
			{ { 1.0f, 1.0f, -1.0f }, { 1.0f, 1.0f, 0.0f } }, // 3
			{ { -1.0f, -1.0f, 1.0f }, { 1.0f, 0.0f, 1.0f } }, // 4
			{ { 1.0f, -1.0f, 1.0f }, { 0.0f, 1.0f, 1.0f } }, // 5
			{ { -1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } }, // 6
			{ { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } } // 7
		};

		indices =
		{
			0, 2, 1,  2, 3, 1,
			1, 3, 5,  3, 7, 5,
			2, 6, 3,  3, 6, 7,
			4, 5, 7,  4, 7, 6,
			0, 4, 2,  2, 4, 6,
			0, 1, 4,  1, 5, 4
		};

		ID3D11Buffer* pVertexBuffer;

		D3D11_BUFFER_DESC vbd = {};
		vbd.ByteWidth = UINT(sizeof(Vertex) * vertices.size());
		vbd.Usage = D3D11_USAGE_DEFAULT;
		vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vbd.CPUAccessFlags = 0;
		vbd.MiscFlags = 0;
		vbd.StructureByteStride = sizeof(Vertex);
		D3D11_SUBRESOURCE_DATA vsd = {};
		vsd.pSysMem = vertices.data();
		renderManager.GetDevice()->CreateBuffer(&vbd, &vsd, &pVertexBuffer);

		const UINT offset = 0;
		UINT stride = sizeof(Vertex);
		renderManager.GetContext()->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
		pVertexBuffer->Release();

		ID3D11Buffer* pIndexBuffer;
		D3D11_BUFFER_DESC ibd = {};
		ibd.ByteWidth = UINT(sizeof(unsigned short) * indices.size());
		ibd.Usage = D3D11_USAGE_DEFAULT;
		ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		ibd.CPUAccessFlags = 0;
		ibd.MiscFlags = 0;
		ibd.StructureByteStride = sizeof(unsigned short);
		D3D11_SUBRESOURCE_DATA isd = {};
		isd.pSysMem = indices.data();
		renderManager.GetDevice()->CreateBuffer(&ibd, &isd, &pIndexBuffer);

		renderManager.GetContext()->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R16_UINT, 0);
		pIndexBuffer->Release();

		WorldViewProj matrices;

		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixRotationX(transform.rot.x));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixRotationY(transform.rot.y));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixRotationZ(transform.rot.z));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixTranslation(transform.pos.x, transform.pos.y, transform.pos.z));
		DirectX::XMStoreFloat4x4(&matrices.wMatrix, world);

		DirectX::XMMATRIX view =
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f });
		DirectX::XMStoreFloat4x4(&matrices.vMatrix, view);

		DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1280.0f / 720.0f, 0.5f, 100.0f);
		DirectX::XMStoreFloat4x4(&matrices.pMatrix, projection);

		ID3D11Buffer* pConstantBuffer;
		D3D11_BUFFER_DESC cbd;
		cbd.ByteWidth = sizeof(matrices);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &matrices;
		renderManager.GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);

		renderManager.GetContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pConstantBuffer->Release();

		ID3D11VertexShader* pVertexShader;
		ID3DBlob* pVSBlob;
		D3DReadFileToBlob(L"VertexShader.cso", &pVSBlob);

		renderManager.GetDevice()->CreateVertexShader(pVSBlob->GetBufferPointer(), pVSBlob->GetBufferSize(), nullptr, &pVertexShader);

		renderManager.GetContext()->VSSetShader(pVertexShader, nullptr, 0);
		pVertexShader->Release();

		ID3D11PixelShader* pPixelShader;
		ID3DBlob* pPSBlob;
		D3DReadFileToBlob(L"PixelShader.cso", &pPSBlob);

		renderManager.GetDevice()->CreatePixelShader(pPSBlob->GetBufferPointer(), pPSBlob->GetBufferSize(), nullptr, &pPixelShader);

		renderManager.GetContext()->PSSetShader(pPixelShader, nullptr, 0);
		pPixelShader->Release();
		pPSBlob->Release();

		ID3D11InputLayout* pInputLayout;

		const std::vector<D3D11_INPUT_ELEMENT_DESC> input =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		renderManager.GetDevice()->CreateInputLayout(
			input.data(), (UINT)input.size(),
			pVSBlob->GetBufferPointer(),
			pVSBlob->GetBufferSize(),
			&pInputLayout
		);
		pVSBlob->Release();

		renderManager.GetContext()->IASetInputLayout(pInputLayout);
		pInputLayout->Release();

		renderManager.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		renderManager.GetContext()->DrawIndexed(indices.size(), 0, 0);
	}

	void Update(RenderManager& renderManager)
	{
		WorldViewProj matrices;

		DirectX::XMMATRIX world = DirectX::XMMatrixIdentity();
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixScaling(transform.scale.x, transform.scale.y, transform.scale.z));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixRotationX(transform.rot.x));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixRotationY(transform.rot.y));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixRotationZ(transform.rot.z));
		world = DirectX::XMMatrixMultiply(world, DirectX::XMMatrixTranslation(transform.pos.x, transform.pos.y, transform.pos.z));
		DirectX::XMStoreFloat4x4(&matrices.wMatrix, world);

		DirectX::XMMATRIX view =
			DirectX::XMMatrixLookAtLH({ 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f, 0.0f });
		DirectX::XMStoreFloat4x4(&matrices.vMatrix, view);

		DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(1.0f, 1280.0f / 720.0f, 0.5f, 100.0f);
		DirectX::XMStoreFloat4x4(&matrices.pMatrix, projection);

		ID3D11Buffer* pConstantBuffer;
		D3D11_BUFFER_DESC cbd;
		cbd.ByteWidth = sizeof(matrices);
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0;
		cbd.StructureByteStride = 0;
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &matrices;
		renderManager.GetDevice()->CreateBuffer(&cbd, &csd, &pConstantBuffer);

		renderManager.GetContext()->VSSetConstantBuffers(0, 1, &pConstantBuffer);
		pConstantBuffer->Release();
	}
};