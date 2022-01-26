#pragma once
#include <vector>
#include "Vertex.h"
#include "Polygon3D.h"
#include "Matrix.h"
#include "Camera.h"
#include "Vector3D.h"
#include "DirectionalLighting.h"
#include "AmbientLighting.h"
#include "PointLighting.h"
#include "UVCoord.h"
#include "Texture.h"

class Model
{
public:

	Model();
	~Model();
	
	/*
	Accesses the information that is included within the current model render
	*/

	const std::vector<Polygon3D>& GetPolygons();
	const std::vector<Vertex>& GetVertices();
	const std::vector<Vertex>& GetTransVertices();
	const std::vector<UVCoord>& GetUVCoords();
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;
	Texture& GetTexture();

	/*
	Loads the information needed about the model into vector 
	collections that we can iterate through to render the model as needed
	*/

	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2, int uvIndex0, int uvIndex1, int uvIndex2);
	void AddTextureUV(float u, float v);

	/*
	Applies the transformation that currently needs to be carried out onto the relevant set of vertices
	Can dehomogenize the vertices to bring them to a 2D plane for rendering on screen
	*/

	void ApplyTransformToLocalVertices(const Matrix& transform);
	void ApplyTransformToTransformedVertices(const Matrix& transform);
	void Dehomogenized();

	/*
	Calculates which polygons need to be culled, 
	depending upon which are "back-facing" to the camera view
	Sorts the polygons (in their collection) so 
	that the first ones to be rendered are the ones that are furthest away (Painters' Sort)
	*/

	void CalculateBackfaces(Camera _camera);
	void Sort(void);
	static bool sortByAvgZ(const Polygon3D& lhs, const Polygon3D& rhs);

	/*
	Calculates the ambient, directional and point lighting that 
	is acting upon the polygon, setting the entire polygon colour once this has been calculated.
	*/

	void CalculateLightingAmbient(const AmbientLighting& ambientLight);
	void CalculateLightingDirectional(const std::vector<DirectionalLighting>& lightingVectors);
	void CalculateLightingPoint(const std::vector<PointLighting>& pointLights);

	/*
	Calculates the ambient, directional and point lighting that
	is acting upon each vertex of a polygon, setting the vertex colours once this has been calculated.
	*/

	void CalculateVertexLightingAmbient(const AmbientLighting& ambientLight);
	void CalculateVertexLightingDirectional(const std::vector<DirectionalLighting>& lightingVectors);
	void CalculateVertexLightingPoint(const std::vector<PointLighting>& pointLights);

	/*
	Calculates the normal to each vector, taking into account how many polygons repeat the vertex, 
	in order to find the perfect normal between each of the shared vertices
	*/

	void CalculateVertexNormal();

private:

	/*
	members for the vector collections needed to hold model data, 
	as well as other members for lighting coefficients
	*/

	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _originalVertices;
	std::vector<Vertex> _transformedVertices;
	std::vector<UVCoord> _uvCoordinates;

	Texture _texture;

	float _ka[3];
	float _kd[3];
	float _ks[3];

};

