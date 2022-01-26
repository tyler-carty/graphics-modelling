#pragma once
#include "Framework.h"
#include "Vertex.h"
#include <vector>
#include "Matrix.h"
#include "MD2Loader.h"
#include "Camera.h"
#include "Model.h"
#include "DirectionalLighting.h"
#include <Windows.h>

class Rasteriser : public Framework
{
public:

	/*
	Definition of the base methods used to run the rasterizer and render models etc...
	*/

	bool Initialise();
	void Update(const Bitmap& bitmap);
	void Render(const Bitmap& bitmap);
	void Shutdown();

	/*
	Generates the perspective and viewing matrices to be taken into account on the model render
	*/

	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

	/*
	Draws the wireframe or solid flat model depending on which is chosen
	*/

	void DrawWireFrame(const Bitmap& bitmap);
	void DrawSolidFlat(const Bitmap& bitmap);

	/*
	Sorts the 3 polygon vertices by Y value to make sure that triangle interpolation works as intended
	*/

	static bool SortByAscY(const Vertex& lhs, const Vertex& rhs);

	/*
	Collection of methods to handle flat shading of each individual pixel in a polygon
	Polygon is split into two triangles if necessary and each is interpolated for XY coords
	*/

	void MyDrawSolidFlat(const Bitmap& bitmap);
	void FillPolygonFlat(const HDC& hdc, std::vector<Vertex>& currentPolygonVertices, const COLORREF& currentColour);
	void FillBottomFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& currentColour);
	void FillTopFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& currentColour);

	/*
	Collection of methods to handle gouraud shading of each individual pixel in a polygon
	Polygon is split into two triangles if necessary and each is interpolated for XY coords and colour values
	*/

	void GouraudShading(const Bitmap& bitmap);
	void FillPolygonGouraud(const HDC& hdc, std::vector<Vertex>& currentPolygonVertices);
	void GouraudFillBottomFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3);
	void GouraudFillTopFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3);

	/*
	Collection of methods to handle flat shading of each individual pixel in a polygon
	Polygon is split into to triangles if necessary and each is interpolated for XY coords and UV coords for texture mapping
	*/

	void DrawSolidTextured(const Bitmap& bitmap);
	void FillSolidTextured(const HDC& hdc, std::vector<Vertex>& currentPolygonVertices);
	void TexturedFillBottomFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3);
	void TexturedFillTopFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3);

private:

	/*
	Members to hold the values needed when rendering a model
	*/

	Model _model;
	Camera _camera;

	std::vector<DirectionalLighting> _lightingVectors;
	std::vector<PointLighting> _lightingPoints;

	float _d;
	int _height;
	int _width;
	float _aspectRatio;

	Matrix modelTransformationMatrix;
	Matrix yRotationMatrix;
	Matrix _currentModelTransformation;
	Matrix perspectiveTransformationMatrix;
	Matrix viewTransformationMatrix;

};

