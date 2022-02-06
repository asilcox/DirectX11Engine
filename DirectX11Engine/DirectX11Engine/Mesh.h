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
protected:
	std::vector<Vertex> vertices;
	std::vector<unsigned short> indices;
};