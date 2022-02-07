#pragma once
class Mesh
{
	friend class RenderManager;
public:
	Mesh(RenderManager& renderManager)
	{};
	std::vector<Vertex> GetVertices()
	{
		return vertices;
	}
	std::vector<unsigned short> GetIndices()
	{
		return indices;
	}
	Transform* GetTransform()
	{
		return &transform;
	}
	void SetTransform(Transform& t)
	{
		transform = t;
	}
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
	Transform transform;
};