#include "Rasteriser.h"
#include <cmath>
#include <algorithm>
#include <wchar.h>

//define the value of pi to use later
#define PI 3.14159265

Rasteriser app;

//define starting values for demonstration
float angle = 0.0f;
float translateValue = 0.0f;
float scaleValue = 0.0f;
int renderCount = 0;

bool Rasteriser::Initialise()
{
	//create camera
	Vertex position = Vertex(0, 0, -50, 1);
	_camera = Camera(0.0f, 0.0f, 0.0f, position);

	//create lights and push onto the relevant collection
	_lightingVectors.push_back(DirectionalLighting(0, 255, 255, (Vector3D(-1, 0, -1))));

	_lightingPoints.push_back(PointLighting(255, 255, 255, (Vertex(0, 0, -50, 1)), 0.0f, 1.0f, 0.0f));

	//load the model and texture, populate collections with vertices, polygons and coords
	if (!MD2Loader::LoadModel("MD2 Files\\marvin.md2" , "Texture Files\\marvin.pcx" , _model,
		&Model::AddPolygon,
		&Model::AddVertex,
		&Model::AddTextureUV))
	{
		return false;
	}

	return true;

} 

void Rasteriser::Update(const Bitmap& bitmap)
{

	//convert angle to radians for matrix maths
	float radians = (float)(angle * PI / 180);

	//loop to render specific transformations relevant to the frame counter
	if (renderCount <= 72)
	{
		//translation
		_currentModelTransformation = { 1, 0, 0, translateValue,
										0, 1, 0, translateValue,
										0, 0, 1, translateValue,
										0, 0, 0, 1 };
	}
	else if (renderCount > 72 && renderCount <= 144)
	{
		//scale
		_currentModelTransformation = { scaleValue, 0, 0, 0,
										0, scaleValue, 0, 0,
										0, 0, scaleValue, 0,
										0, 0, 0, 1};
	}
	else if (renderCount > 144 && renderCount <= 216)
	{
		//x rotation
		_currentModelTransformation = { 1, 0, 0, 0,
										0, cos(radians), sin(-radians), 0,
										0, sin(radians), cos(radians), 0,
										0, 0, 0, 1};
	}
	else if (renderCount > 216 && renderCount <= 288)
	{
		//y rotation
		_currentModelTransformation = { cos(radians), 0, sin(radians), 0,
										 0, 1, 0, 0,
										 sin(-radians), 0, cos(radians), 0,
										 0, 0, 0, 1 };
	}
	else if (renderCount > 288 && renderCount <= 360)
	{
		//z rotation
		_currentModelTransformation = { cos(radians), sin(radians), 0, 0,
										 sin(-radians), cos(radians), 0, 0,
										 0, 0, 1, 0,
										 0, 0, 0, 1 };
	}
	else {
		//y rotation
		_currentModelTransformation = { cos(radians), 0, sin(radians), 0,
										 0, 1, 0, 0,
										 sin(-radians), 0, cos(radians), 0,
										 0, 0, 0, 1 };
	}

	_d = 1;

	//calculate the aspect ratio of window to consider when multiplying matrices
	_width = bitmap.GetWidth();
	_height = bitmap.GetHeight();

	_aspectRatio = float(_width) / float(_height);

	//generate the relevant matrices dependant on these values
	GeneratePerspectiveMatrix(_d, _aspectRatio);
	GenerateViewMatrix(_d, _width, _height);

	//increment the transformation values
	angle++;
	translateValue++;
	scaleValue += 0.01f;
}

void Rasteriser::Render(const Bitmap& bitmap)
{
	//clear window of old drawings
	bitmap.Clear(RGB (0, 0, 0));

	//apply transformations, back-face culling, sorting, lighting, and dehomogenization to all relevant collections before drawing
	_model.ApplyTransformToLocalVertices(_currentModelTransformation);
	_model.CalculateBackfaces(_camera);
	_model.CalculateVertexNormal();
	_model.CalculateLightingAmbient(AmbientLighting(32, 32, 32));
	_model.CalculateLightingDirectional(_lightingVectors);
	_model.CalculateLightingPoint(_lightingPoints);
	_model.CalculateVertexLightingAmbient(AmbientLighting(32, 32, 32));
	_model.CalculateVertexLightingDirectional(_lightingVectors);
	_model.CalculateVertexLightingPoint(_lightingPoints);
	_model.ApplyTransformToTransformedVertices(_camera.CreateViewingMatrix());
	_model.Sort();
	_model.ApplyTransformToTransformedVertices(perspectiveTransformationMatrix);
	_model.Dehomogenized();
	_model.ApplyTransformToTransformedVertices(viewTransformationMatrix);

	//Draw specific model type dependant on the frame counter. Resets when last model has been drawn and rotated around a little

	if (renderCount <= 360) 
	{
		//draws wireframe with BF culling
		renderCount++;
		DrawWireFrame(bitmap);
	}
	else if (renderCount > 360 && renderCount <= 480)
	{
		//draws solid flat with constant colour
		renderCount++;
		DrawSolidFlat(bitmap);
	}
	else if (renderCount > 480 && renderCount <= 600)
	{
		//draws solid flat with specific polygon colour after light calculations
		renderCount++;
		DrawSolidFlat(bitmap);
	}
	else if (renderCount > 600 && renderCount <= 660)
	{
		//draws solid flat (per pixel) using my custom polygon interpolation method
		renderCount++;
		MyDrawSolidFlat(bitmap);
	}
	else if (renderCount > 660 && renderCount <= 720)
	{
		//draws smooth shaded model (gouraud) using my custom polygon interpolation method for colour and vertex values
		renderCount++;
		GouraudShading(bitmap);
	}
	else if (renderCount > 720 && renderCount <= 779) 
	{
		//draws texture mapped model
		renderCount++;
		DrawSolidTextured(bitmap);
	}
	else 
	{
		//resets the counter once all is done
		renderCount = 0;
		translateValue = 0;
		scaleValue = 0.0f;
	}

}

void Rasteriser::Shutdown()
{
}

void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	_aspectRatio = aspectRatio;

	//generates perspective matrix using D and the aspect ratio
	perspectiveTransformationMatrix = Matrix{ d / _aspectRatio, 0, 0, 0,
											   0, d, 0, 0,
											   0, 0, d, 0,
											   0, 0, d, 0 };

}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	//generates view matrix using D and the width/height of the window
	viewTransformationMatrix = Matrix{ width / 2.0f, 0.0f, 0.0f, width / 2.0f,
								   0, -height / 2.0f, 0, height / 2.0f,
								   0, 0, d / 2, d / 2,
								   0, 0, 1, 0 };
}

void Rasteriser::DrawWireFrame(const Bitmap& bitmap)
{
	//gets device context
	HDC hdc = bitmap.GetDC();

	//draws specific label depending on which transformation is shown
	if (renderCount <= 72)
	{
		const wchar_t* text = L"Wireframe with Culling and Depth Sorting, Shows Translation in all 3 Axis";
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, text, lstrlen(text));
	}
	else if (renderCount > 72 && renderCount <= 144)
	{
		const wchar_t* text = L"Wireframe with Culling and Depth Sorting, Shows Scaling in all 3 Axis";
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, text, lstrlen(text));
	}
	else if (renderCount > 144 && renderCount <= 216)
	{
		const wchar_t* text = L"Wireframe with Culling and Depth Sorting, Shows X Rotation";
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, text, lstrlen(text));
	}
	else if (renderCount > 216 && renderCount <= 288)
	{
		const wchar_t* text = L"Wireframe with Culling and Depth Sorting, Shows Y Rotation";
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, text, lstrlen(text));
	}
	else if (renderCount > 288 && renderCount <= 360)
	{
		const wchar_t* text = L"Wireframe with Culling and Depth Sorting, Shows Z Rotation";
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		TextOut(hdc, 0, 0, text, lstrlen(text));
	}

	//create vector list of polygons using GetPolygons method

	std::vector<Polygon3D> localPolygonList = _model.GetPolygons();

	//create vector list of vertices using GetVertices method

	std::vector<Vertex> localVerticesCollection = _model.GetTransVertices();

	//loop through each polygon in this collection

	for (int i = 0; i < localPolygonList.size(); i++)
	{
		if (localPolygonList[i].GetCullState() == false) {

			//get the index of each vertex in the polygon list

			int i0 = localPolygonList[i].GetIndex(0);
			int i1 = localPolygonList[i].GetIndex(1);
			int i2 = localPolygonList[i].GetIndex(2);

			//save the vertices, referenced with the indices above, into unique variables.

			Vertex vertex1 = localVerticesCollection[i0];
			Vertex vertex2 = localVerticesCollection[i1];
			Vertex vertex3 = localVerticesCollection[i2];

			std::vector<Vertex> localPolygonVertexCollection;

			localPolygonVertexCollection.push_back(vertex1);
			localPolygonVertexCollection.push_back(vertex2);
			localPolygonVertexCollection.push_back(vertex3);

			//draw the polygon


			SelectObject(hdc, GetStockObject(WHITE_PEN));

			for (int j = 0; j <= 2; j++)
			{
				if (j == 0)
				{
					MoveToEx(hdc, (int)localPolygonVertexCollection[j].GetX(), (int)localPolygonVertexCollection[j].GetY(), NULL);
				}
				else
				{
					LineTo(hdc, (int)localPolygonVertexCollection[j].GetX(), (int)localPolygonVertexCollection[j].GetY());
				}
			}
			LineTo(hdc, (int)localPolygonVertexCollection[0].GetX(), (int)localPolygonVertexCollection[0].GetY());

			DeleteObject(GetStockObject(WHITE_PEN));

		}
	}
}

void Rasteriser::DrawSolidFlat(const Bitmap& bitmap)
{

	//define current colour
	COLORREF currentColour = (0, 0, 0);

	//gets polygons
	std::vector<Polygon3D> localPolygonList = _model.GetPolygons();

	//loop though all polygons, draw if not culled
	for (int i = 0; i < localPolygonList.size(); i++) 
	{
		if (localPolygonList[i].GetCullState() == false) 
		{

			//gets index of vertex
			int i0 = localPolygonList[i].GetIndex(0);
			int i1 = localPolygonList[i].GetIndex(1);
			int i2 = localPolygonList[i].GetIndex(2);

			//create vector list of vertices using GetVertices method

			std::vector<Vertex> localVerticesCollection = _model.GetTransVertices();

			//save the vertices, referenced with the indices above, into unique variables.

			Vertex vertex1 = localVerticesCollection[i0];
			Vertex vertex2 = localVerticesCollection[i1];
			Vertex vertex3 = localVerticesCollection[i2];

			//creates 3 POINT objects for the polygon fill method
			POINT points[3];
			points[0] = { long(vertex1.GetX()) , long(vertex1.GetY()) };
			points[1] = { long(vertex2.GetX()) , long(vertex2.GetY()) };
			points[2] = { long(vertex3.GetX()) , long(vertex3.GetY()) };

			//gets DC
			HDC hdc = bitmap.GetDC();

			//changes the label/colour depending on the currently shown model type
			if (renderCount <= 480) {

				const wchar_t* text = L"Flat Shading with Constant Colour";
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, TRANSPARENT);
				TextOut(hdc, 0, 0, text, lstrlen(text));

				currentColour = RGB(0, 255, 255);
			}
			else {
				const wchar_t* text = L"Flat Shading with Ambient, Directional and Point Lighting Accounted For";
				SetTextColor(hdc, RGB(255, 255, 255));
				SetBkMode(hdc, TRANSPARENT);
				TextOut(hdc, 0, 0, text, lstrlen(text));

				currentColour = localPolygonList[i].GetRGBValue();
			}

			//creates brush as a colour decided above
			HBRUSH fillBrush = CreateSolidBrush(currentColour);
			HPEN polygonPen = CreatePen(PS_SOLID, 2, currentColour);

			SelectObject(hdc, polygonPen);
			SelectObject(hdc, fillBrush);

			//fills polygons with correct colour
			Polygon(hdc, points, 3);

			DeleteObject(polygonPen);
			DeleteObject(fillBrush);

		}
	}

}

void Rasteriser::MyDrawSolidFlat(const Bitmap& bitmap)
{
	//gets polygons
	std::vector<Polygon3D> localPolygonList = _model.GetPolygons();

	//loops through polygons, draws if culling is false
	for (int i = 0; i < localPolygonList.size(); i++)
	{
		if (localPolygonList[i].GetCullState() == false)
		{

			int i0 = localPolygonList[i].GetIndex(0);
			int i1 = localPolygonList[i].GetIndex(1);
			int i2 = localPolygonList[i].GetIndex(2);

			//create vector list of vertices using GetVertices method

			std::vector<Vertex> localVerticesCollection = _model.GetTransVertices();

			//save the vertices, referenced with the indices above, into unique variables.

			Vertex vertex1 = localVerticesCollection[i0];
			Vertex vertex2 = localVerticesCollection[i1];
			Vertex vertex3 = localVerticesCollection[i2];

			std::vector<Vertex> currentPolygonVertices;

			currentPolygonVertices.push_back(vertex1);
			currentPolygonVertices.push_back(vertex2);
			currentPolygonVertices.push_back(vertex3);

			//gets DC
			HDC hdc = bitmap.GetDC();

			//sets label to show what is happening
			const wchar_t* text = L"Flat Shading / Lighting with My Own Polygon Fill Method";
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 0, 0, text, lstrlen(text));

			//decides current colour
			COLORREF currentColour = localPolygonList[i].GetRGBValue();

			//uses my method to fill a polygon (flat shaded)
			FillPolygonFlat(hdc, currentPolygonVertices, currentColour);
		}
	}

}

void Rasteriser::FillPolygonFlat(const HDC& hdc, std::vector<Vertex>& currentPolygonVertices, const COLORREF& currentColour)
{

	//sorts vertices by Y
	sort(currentPolygonVertices.begin(), currentPolygonVertices.end(), SortByAscY);

	//gets the 3 vertices
	Vertex vertex1 = currentPolygonVertices[0];
	Vertex vertex2 = currentPolygonVertices[1];
	Vertex vertex3 = currentPolygonVertices[2];

	//decides whether a polygon needs to be split
	if (vertex2.GetY() == vertex3.GetY())
	{
		FillBottomFlatTriangle(hdc, vertex1, vertex2, vertex3, currentColour);
	}
	else if (vertex1.GetY() == vertex2.GetY())
	{
		FillTopFlatTriangle(hdc, vertex1, vertex2, vertex3, currentColour);
	}
	else
	{
		//interpolates a 4th vertex to split the triangle
		Vertex vertex4 = Vertex((vertex1.GetIntX() + 
								((float)(vertex2.GetIntY() - vertex1.GetIntY()) / 
								(float)(vertex3.GetIntY() - vertex1.GetIntY())) * 
								(vertex3.GetIntX() - vertex1.GetIntX())), 
								(float)vertex2.GetIntY(), 1, 1);

		//calls both methods
		FillBottomFlatTriangle(hdc, vertex1, vertex2, vertex4, currentColour);
		FillTopFlatTriangle(hdc, vertex2, vertex4, vertex3, currentColour);

	}

}

//binary predicate to sort by Y
bool Rasteriser::SortByAscY(const Vertex& lhs, const Vertex& rhs)
{

	return lhs.GetIntY() < rhs.GetIntY();

}

void Rasteriser::FillBottomFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& currentColour)
{
	//gets gradient of change for triangel slopes
	float invSlope1 = (float)(vertex2.GetIntX() - vertex1.GetIntX()) / (float)(vertex2.GetIntY() - vertex1.GetIntY());
	float invSlope2 = (float)(vertex3.GetIntX() - vertex1.GetIntX()) / (float)(vertex3.GetIntY() - vertex1.GetIntY());

	//gets starting X values
	float currentX1 = (float)vertex1.GetIntX();
	float currentX2 = (float)vertex1.GetIntX() + 0.5f;

	//switches slope if they are backwards
	if (invSlope2 < invSlope1)
	{
		float slopeTemp = invSlope1;
		invSlope1 = invSlope2;
		invSlope2 = slopeTemp;
	}

	//loops through all rows to set each pixel to the relevant colour
	for (int scanlineY = int(vertex1.GetIntY()); scanlineY <= vertex2.GetIntY(); scanlineY++)
	{

		for (int xPos = (int)ceil(currentX1); xPos < currentX2; xPos++)
		{
			SetPixel(hdc, xPos, scanlineY, currentColour);
		}

		//increments x value to go to next one
		currentX1 += invSlope1;
		currentX2 += invSlope2;

	}

}

void Rasteriser::FillTopFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& currentColour)
{
	//gets gradient of change for triangel slopes
	float invSlope1 = (float)(vertex3.GetIntX() - vertex1.GetIntX()) / (float)(vertex3.GetIntY() - vertex1.GetIntY());
	float invSlope2 = (float)(vertex3.GetIntX() - vertex2.GetIntX()) / (float)(vertex3.GetIntY() - vertex2.GetIntY());

	//gets starting X values
	float currentX1 = (float)vertex3.GetIntX();
	float currentX2 = (float)vertex3.GetIntX() + 0.5f;

	//flips grandients if backwards
	if (invSlope1 < invSlope2)
	{
		float slopeTemp = invSlope1;
		invSlope1 = invSlope2;
		invSlope2 = slopeTemp;
	}

	//loops though each row and pixel to set colour
	for (int scanlineY = int(vertex3.GetIntY()); scanlineY > vertex1.GetIntY(); scanlineY--)
	{

		for (int xPos = (int)ceil(currentX1); xPos < currentX2; xPos++)
		{
			SetPixel(hdc, xPos, scanlineY, currentColour);
		}

		//decrements X value for next value
		currentX1 -= invSlope1;
		currentX2 -= invSlope2;

	}

}

void Rasteriser::GouraudShading(const Bitmap& bitmap)
{
	//gets polygons
	std::vector<Polygon3D> localPolygonList = _model.GetPolygons();

	//for each polygon, if not culled
	for (int i = 0; i < localPolygonList.size(); i++)
	{
		if (localPolygonList[i].GetCullState() == false)
		{

			int i0 = localPolygonList[i].GetIndex(0);
			int i1 = localPolygonList[i].GetIndex(1);
			int i2 = localPolygonList[i].GetIndex(2);

			//create vector list of vertices using GetVertices method

			std::vector<Vertex> localVerticesCollection = _model.GetTransVertices();

			//save the vertices, referenced with the indices above, into unique variables.

			Vertex vertex1 = localVerticesCollection[i0];
			Vertex vertex2 = localVerticesCollection[i1];
			Vertex vertex3 = localVerticesCollection[i2];

			std::vector<Vertex> currentPolygonVertices;

			currentPolygonVertices.push_back(vertex1);
			currentPolygonVertices.push_back(vertex2);
			currentPolygonVertices.push_back(vertex3);

			//gets DC
			HDC hdc = bitmap.GetDC();

			//draws a label to show what is drawn
			const wchar_t* text = L"Gouraud Smooth Shading with Lighting Accounted For";
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 0, 0, text, lstrlen(text));

			//calls my method to fill a polygon with smooth shaded colours
			FillPolygonGouraud(hdc, currentPolygonVertices);
		}
	}
}

void Rasteriser::FillPolygonGouraud(const HDC& hdc, std::vector<Vertex>& currentPolygonVertices)
{
	//sorts vertices by asc Y
	sort(currentPolygonVertices.begin(), currentPolygonVertices.end(), SortByAscY);

	//gets vertices
	Vertex vertex1 = currentPolygonVertices[0];
	Vertex vertex2 = currentPolygonVertices[1];
	Vertex vertex3 = currentPolygonVertices[2];

	//gets colour of each vertex
	COLORREF vertColour1 = vertex1.GetVertexRGB();
	COLORREF vertColour2 = vertex2.GetVertexRGB();
	COLORREF vertColour3 = vertex3.GetVertexRGB();

	//decided what type of triangle we are dealing with
	if (vertex2.GetIntY() == vertex3.GetIntY())
	{
		GouraudFillBottomFlatTriangle(hdc, vertex1, vertex2, vertex3, vertColour1, vertColour2, vertColour3);
	}
	else if (vertex1.GetIntY() == vertex2.GetIntY())
	{
		GouraudFillTopFlatTriangle(hdc, vertex1, vertex2, vertex3, vertColour1, vertColour2, vertColour3);
	}
	else
	{
		//interpolates 4th vertex
		Vertex vTmp = Vertex((vertex1.GetIntX() + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (vertex3.GetIntX() - vertex1.GetIntX())), (float)vertex2.GetIntY(), 1, 1);

		//interpolates the colour at this 4th vertex
		float cBlue = GetBValue(vertColour1) + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (GetBValue(vertColour3) - GetBValue(vertColour1));
		float cRed = GetRValue(vertColour1) + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (GetRValue(vertColour3) - GetRValue(vertColour1));
		float cGreen = GetGValue(vertColour1) + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (GetGValue(vertColour3) - GetGValue(vertColour1));
		COLORREF cTmp = RGB(cRed, cGreen, cBlue);

		//calls methods to fill
		GouraudFillBottomFlatTriangle(hdc, vertex1, vertex2, vTmp, vertColour1, vertColour2, cTmp);
		GouraudFillTopFlatTriangle(hdc, vertex2, vTmp, vertex3, vertColour2, cTmp, vertColour3);

	}
}

void Rasteriser::GouraudFillBottomFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3)
{
	//gets slope of change in x
	float invSlope1 = (float)(vertex2.GetIntX() - vertex1.GetIntX()) / (float)(vertex2.GetIntY() - vertex1.GetIntY());
	float invSlope2 = (float)(vertex3.GetIntX() - vertex1.GetIntX()) / (float)(vertex3.GetIntY() - vertex1.GetIntY());

	//gets starting x values
	float currentX1 = (float)vertex1.GetIntX();
	float currentX2 = (float)vertex1.GetIntX() + 0.5f;

	//gets slopes of colour change using difference in Y
	float v2v1Diff = (float)(vertex2.GetIntY() - vertex1.GetIntY());
	float colorSlopeBlue1 = (float)(GetBValue(vertColour2) - GetBValue(vertColour1)) / v2v1Diff;
	float colorSlopeRed1 = (float)(GetRValue(vertColour2) - GetRValue(vertColour1)) / v2v1Diff;
	float colorSlopeGreen1 = (float)(GetGValue(vertColour2) - GetGValue(vertColour1)) / v2v1Diff;

	float v3v1Diff = (float)(vertex3.GetIntY() - vertex1.GetIntY());
	float colorSlopeBlue2 = (float)(GetBValue(vertColour3) - GetBValue(vertColour1)) / v3v1Diff;
	float colorSlopeRed2 = (float)(GetRValue(vertColour3) - GetRValue(vertColour1)) / v3v1Diff;
	float colorSlopeGreen2 = (float)(GetGValue(vertColour3) - GetGValue(vertColour1)) / v3v1Diff;

	//sets colours to start with
	float cBlue1 = GetBValue(vertColour1);
	float cRed1 = GetRValue(vertColour1);
	float cGreen1 = GetGValue(vertColour1);
	float cBlue2 = GetBValue(vertColour1);
	float cRed2 = GetRValue(vertColour1);
	float cGreen2 = GetGValue(vertColour1);

	//switches slopes if wrong way round
	if (invSlope2 < invSlope1)
	{
		float slopeTemp = invSlope1;
		invSlope1 = invSlope2;
		invSlope2 = slopeTemp;

		slopeTemp = colorSlopeRed1;
		colorSlopeRed1 = colorSlopeRed2;
		colorSlopeRed2 = slopeTemp;

		slopeTemp = colorSlopeGreen1;
		colorSlopeGreen1 = colorSlopeGreen2;
		colorSlopeGreen2 = slopeTemp;

		slopeTemp = colorSlopeBlue1;
		colorSlopeBlue1 = colorSlopeBlue2;
		colorSlopeBlue2 = slopeTemp;
	}

	//loops though every lines, every pixel on that line
	for (int scanlineY = vertex1.GetIntY(); scanlineY <= vertex2.GetIntY(); scanlineY++)
	{
		for (int xPos = (int)ceil(currentX1); xPos < currentX2; xPos++)
		{
			float t = (xPos - currentX1) / (currentX2 - currentX1);

			//interpolates colour
			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			//sets pixel to colour
			COLORREF currentPixelColour = RGB(red, green, blue);

			SetPixel(hdc, xPos, scanlineY, currentPixelColour);
		}

		//increments slopes for next pass
		currentX1 += invSlope1;
		currentX2 += invSlope2;

		cRed1 += colorSlopeRed1;
		cGreen1 += colorSlopeGreen1;
		cBlue1 += colorSlopeBlue1;

		cRed2 += colorSlopeRed2;
		cGreen2 += colorSlopeGreen2;
		cBlue2 += colorSlopeBlue2;
	}

}

void Rasteriser::GouraudFillTopFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3)
{
	//gets slopes for change in X
	float invSlope1 = (float)(vertex3.GetIntX() - vertex1.GetIntX()) / (float)(vertex3.GetIntY() - vertex1.GetIntY());
	float invSlope2 = (float)(vertex3.GetIntX() - vertex2.GetIntX()) / (float)(vertex3.GetIntY() - vertex2.GetIntY());

	//gets starting X values
	float currentX1 = (float)vertex3.GetIntX();
	float currentX2 = (float)vertex3.GetIntX() + 0.5f;

	//gets colour slopes for change in Y
	float v3v1Diff = (float)(vertex3.GetIntY() - vertex1.GetIntY());
	float colorSlopeBlue2 = (float)(GetBValue(vertColour3) - GetBValue(vertColour1)) / v3v1Diff;
	float colorSlopeRed2 = (float)(GetRValue(vertColour3) - GetRValue(vertColour1)) / v3v1Diff;
	float colorSlopeGreen2 = (float)(GetGValue(vertColour3) - GetGValue(vertColour1)) / v3v1Diff;

	float v3v2Diff = (float)(vertex3.GetIntY() - vertex2.GetIntY());
	float colorSlopeBlue1 = (float)(GetBValue(vertColour3) - GetBValue(vertColour2)) / v3v2Diff;
	float colorSlopeRed1 = (float)(GetRValue(vertColour3) - GetRValue(vertColour2)) / v3v2Diff;
	float colorSlopeGreen1 = (float)(GetGValue(vertColour3) - GetGValue(vertColour2)) / v3v2Diff;

	//gets colours to start with
	float cBlue1 = GetBValue(vertColour3);
	float cRed1 = GetRValue(vertColour3);
	float cGreen1 = GetGValue(vertColour3);
	float cBlue2 = GetBValue(vertColour3);
	float cRed2 = GetRValue(vertColour3);
	float cGreen2 = GetGValue(vertColour3);

	//if slopes are backwards, swap them
	if (invSlope2 < invSlope1)
	{
		float slopeTemp = invSlope1;
		invSlope1 = invSlope2;
		invSlope2 = slopeTemp;

		slopeTemp = colorSlopeRed1;
		colorSlopeRed1 = colorSlopeRed2;
		colorSlopeRed2 = slopeTemp;

		slopeTemp = colorSlopeGreen1;
		colorSlopeGreen1 = colorSlopeGreen2;
		colorSlopeGreen2 = slopeTemp;

		slopeTemp = colorSlopeBlue1;
		colorSlopeBlue1 = colorSlopeBlue2;
		colorSlopeBlue2 = slopeTemp;
	}

	//for each pixel on each line
	for (int scanlineY = vertex3.GetIntY(); scanlineY > vertex1.GetIntY(); scanlineY--)
	{

		for (int xPos = (int)ceil(currentX1); xPos < currentX2; xPos++)
		{
			float t = (xPos - currentX1) / (currentX2 - currentX1);

			//interpolate colour
			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			//set pixel to colour
			COLORREF currentPixelColour = RGB(red, green, blue);
			SetPixel(hdc, xPos, scanlineY, currentPixelColour);
		}

		//decrement slopes for next pass
		currentX1 -= invSlope2;
		currentX2 -= invSlope1;

		cRed1 -= colorSlopeRed1;
		cGreen1 -= colorSlopeGreen1;
		cBlue1 -= colorSlopeBlue1;

		cRed2 -= colorSlopeRed2;
		cGreen2 -= colorSlopeGreen2;
		cBlue2 -= colorSlopeBlue2;
	}

}

void Rasteriser::DrawSolidTextured(const Bitmap& bitmap)
{
	//gets polygons and UV coords
	std::vector<Polygon3D> localPolygonList = _model.GetPolygons();
	std::vector<UVCoord> localUVCoordList = _model.GetUVCoords();

	for (int i = 0; i < localPolygonList.size(); i++)
	{
		if (localPolygonList[i].GetCullState() == false)
		{

			int i0 = localPolygonList[i].GetIndex(0);
			int i1 = localPolygonList[i].GetIndex(1);
			int i2 = localPolygonList[i].GetIndex(2);

			//create vector list of vertices using GetVertices method

			std::vector<Vertex> localVerticesCollection = _model.GetTransVertices();

			//save the vertices, referenced with the indices above, into unique variables.

			Vertex vertex1 = localVerticesCollection[i0];
			Vertex vertex2 = localVerticesCollection[i1];
			Vertex vertex3 = localVerticesCollection[i2];

			//sets vertex UV coord temporarily
			vertex1.SetUVCoord(localUVCoordList[localPolygonList[i].GetUVIndex(0)]);
			vertex2.SetUVCoord(localUVCoordList[localPolygonList[i].GetUVIndex(1)]);
			vertex3.SetUVCoord(localUVCoordList[localPolygonList[i].GetUVIndex(2)]);

			std::vector<Vertex> currentPolygonVertices;

			currentPolygonVertices.push_back(vertex1);
			currentPolygonVertices.push_back(vertex2);
			currentPolygonVertices.push_back(vertex3);

			//calculates interpolation values to be used per vertex
			float uOverZ = currentPolygonVertices[0].GetUVCoord().GetIntU() / currentPolygonVertices[0].GetPreTranZ();
			float vOverZ = currentPolygonVertices[0].GetUVCoord().GetIntV() / currentPolygonVertices[0].GetPreTranZ();
			float zRecip = 1 / currentPolygonVertices[0].GetPreTranZ();

			//sets these values
			currentPolygonVertices[0].SetUOZ(uOverZ);
			currentPolygonVertices[0].SetVOZ(vOverZ);
			currentPolygonVertices[0].SetZR(zRecip);

			uOverZ = currentPolygonVertices[1].GetUVCoord().GetIntU() / currentPolygonVertices[1].GetPreTranZ();
			vOverZ = currentPolygonVertices[1].GetUVCoord().GetIntV() / currentPolygonVertices[1].GetPreTranZ();
			zRecip = 1 / currentPolygonVertices[1].GetPreTranZ();

			currentPolygonVertices[1].SetUOZ(uOverZ);
			currentPolygonVertices[1].SetVOZ(vOverZ);
			currentPolygonVertices[1].SetZR(zRecip);

			uOverZ = currentPolygonVertices[2].GetUVCoord().GetIntU() / currentPolygonVertices[2].GetPreTranZ();
			vOverZ = currentPolygonVertices[2].GetUVCoord().GetIntV() / currentPolygonVertices[2].GetPreTranZ();
			zRecip = 1 / currentPolygonVertices[2].GetPreTranZ();

			currentPolygonVertices[2].SetUOZ(uOverZ);
			currentPolygonVertices[2].SetVOZ(vOverZ);
			currentPolygonVertices[2].SetZR(zRecip);

			//gets DC
			HDC hdc = bitmap.GetDC();

			//Draws correct label
			const wchar_t* text = L"Texture Mapping to Model";
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkMode(hdc, TRANSPARENT);
			TextOut(hdc, 0, 0, text, lstrlen(text));

			//calls texture mapping method
			FillSolidTextured(hdc, currentPolygonVertices);
		}
	}
}

void Rasteriser::FillSolidTextured(const HDC& hdc, std::vector<Vertex>& currentPolygonVertices)
{
	//sorts vertices by ASC Y
	sort(currentPolygonVertices.begin(), currentPolygonVertices.end(), SortByAscY);

	Vertex vertex1 = currentPolygonVertices[0];
	Vertex vertex2 = currentPolygonVertices[1];
	Vertex vertex3 = currentPolygonVertices[2];

	COLORREF vertColour1 = vertex1.GetVertexRGB();
	COLORREF vertColour2 = vertex2.GetVertexRGB();
	COLORREF vertColour3 = vertex3.GetVertexRGB();

	//decides which colour we are dealing with
	if (vertex2.GetIntY() == vertex3.GetIntY())
	{
		TexturedFillBottomFlatTriangle(hdc, vertex1, vertex2, vertex3, vertColour1, vertColour2, vertColour3);
	}
	else if (vertex1.GetIntY() == vertex2.GetIntY())
	{
		TexturedFillTopFlatTriangle(hdc, vertex1, vertex2, vertex3, vertColour1, vertColour2, vertColour3);
	}
	else
	{
		//interpolates vertex 4, with colour and UV values
		Vertex vertTmp = Vertex((vertex1.GetIntX() + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (vertex3.GetIntX() - vertex1.GetIntX())), (float)vertex2.GetIntY(), 1, 1);

		float cBlue = GetBValue(vertColour1) + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (GetBValue(vertColour3) - GetBValue(vertColour1));
		float cRed = GetRValue(vertColour1) + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (GetRValue(vertColour3) - GetRValue(vertColour1));
		float cGreen = GetGValue(vertColour1) + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (GetGValue(vertColour3) - GetGValue(vertColour1));
		COLORREF cTmp = RGB(cRed, cGreen, cBlue);

		float uTmp = vertex1.GetUVCoord().GetIntU() + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (vertex3.GetUVCoord().GetIntU() - vertex1.GetUVCoord().GetIntU());
		float vTmp = vertex1.GetUVCoord().GetIntV() + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (vertex3.GetUVCoord().GetIntV() - vertex1.GetUVCoord().GetIntV());
		float preTranZTmp = vertex1.GetPreTranZ() + ((float)(vertex2.GetIntY() - vertex1.GetIntY()) / (float)(vertex3.GetIntY() - vertex1.GetIntY())) * (vertex3.GetPreTranZ() - vertex1.GetPreTranZ());

		vertTmp.SetUOZ(uTmp / preTranZTmp);
		vertTmp.SetVOZ(vTmp / preTranZTmp);
		vertTmp.SetZR(1 / preTranZTmp);

		TexturedFillBottomFlatTriangle(hdc, vertex1, vertex2, vertTmp, vertColour1, vertColour2, cTmp);
		TexturedFillTopFlatTriangle(hdc, vertex2, vertTmp, vertex3, vertColour2, cTmp, vertColour3);

	}
}

void Rasteriser::TexturedFillBottomFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3)
{
	//gets slope of change in X
	float invSlope1 = (float)(vertex2.GetIntX() - vertex1.GetIntX()) / (float)(vertex2.GetIntY() - vertex1.GetIntY());
	float invSlope2 = (float)(vertex3.GetIntX() - vertex1.GetIntX()) / (float)(vertex3.GetIntY() - vertex1.GetIntY());

	//gets starting X values
	float currentX1 = (float)vertex1.GetIntX();
	float currentX2 = (float)vertex1.GetIntX() + 0.5f;

	//gets colour and calculation slopes depending on change in Y
	float v2v1Diff = (float)(vertex2.GetIntY() - vertex1.GetIntY());
	float colorSlopeBlue1 = (float)(GetBValue(vertColour2) - GetBValue(vertColour1)) / v2v1Diff;
	float colorSlopeRed1 = (float)(GetRValue(vertColour2) - GetRValue(vertColour1)) / v2v1Diff;
	float colorSlopeGreen1 = (float)(GetGValue(vertColour2) - GetGValue(vertColour1)) / v2v1Diff;
	float uozSlope1 = (float)(vertex2.GetUOZ() - vertex1.GetUOZ()) / v2v1Diff;
	float vozSlope1 = (float)(vertex2.GetVOZ() - vertex1.GetVOZ()) / v2v1Diff;
	float zrSlope1 = (float)(vertex2.GetZR() - vertex1.GetZR()) / v2v1Diff;

	float v3v1Diff = (float)(vertex3.GetIntY() - vertex1.GetIntY());
	float colorSlopeBlue2 = (float)(GetBValue(vertColour3) - GetBValue(vertColour1)) / v3v1Diff;
	float colorSlopeRed2 = (float)(GetRValue(vertColour3) - GetRValue(vertColour1)) / v3v1Diff;
	float colorSlopeGreen2 = (float)(GetGValue(vertColour3) - GetGValue(vertColour1)) / v3v1Diff;
	float uozSlope2 = (float)(vertex3.GetUOZ() - vertex1.GetUOZ()) / v3v1Diff;
	float vozSlope2 = (float)(vertex3.GetVOZ() - vertex1.GetVOZ()) / v3v1Diff;
	float zrSlope2 = (float)(vertex3.GetZR() - vertex1.GetZR()) / v3v1Diff;

	//gets starting colour/calculation values
	float cBlue1 = GetBValue(vertColour1);
	float cRed1 = GetRValue(vertColour1);
	float cGreen1 = GetGValue(vertColour1);
	float cBlue2 = GetBValue(vertColour1);
	float cRed2 = GetRValue(vertColour1);
	float cGreen2 = GetGValue(vertColour1);

	float cU1 = (float)vertex1.GetUOZ();
	float cU2 = (float)vertex1.GetUOZ();
	float cV1 = (float)vertex1.GetVOZ();
	float cV2 = (float)vertex1.GetVOZ();
	float cZ1 = (float)vertex1.GetZR();
	float cZ2 = (float)vertex1.GetZR();

	//if slopes are backwards, switch them
	if (invSlope2 < invSlope1)
	{
		float slopeTemp = invSlope1;
		invSlope1 = invSlope2;
		invSlope2 = slopeTemp;

		slopeTemp = colorSlopeRed1;
		colorSlopeRed1 = colorSlopeRed2;
		colorSlopeRed2 = slopeTemp;

		slopeTemp = colorSlopeGreen1;
		colorSlopeGreen1 = colorSlopeGreen2;
		colorSlopeGreen2 = slopeTemp;

		slopeTemp = colorSlopeBlue1;
		colorSlopeBlue1 = colorSlopeBlue2;
		colorSlopeBlue2 = slopeTemp;

		slopeTemp = uozSlope1;
		uozSlope1 = uozSlope2;
		uozSlope2 = slopeTemp;

		slopeTemp = vozSlope1;
		vozSlope1 = vozSlope2;
		vozSlope2 = slopeTemp;

		slopeTemp = zrSlope1;
		zrSlope1 = zrSlope2;
		zrSlope2 = slopeTemp;
	}

	//for each pixel on each line
	for (int scanlineY = vertex1.GetIntY(); scanlineY <= vertex2.GetIntY(); scanlineY++)
	{
		for (int xPos = (int)ceil(currentX1); xPos < currentX2; xPos++)
		{
			float t = (xPos - currentX1) / (currentX2 - currentX1);

			//interpolate colour and calculation values
			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			float uoz = (1 - t) * cU1 + t * cU2;
			float voz = (1 - t) * cV1 + t * cV2;
			float zr = (1 - t) * cZ1 + t * cZ2;

			//convert calc values back to UV coords
			float u = uoz / zr;
			float v = voz / zr;

			COLORREF lightingColour = RGB(red, green, blue);

			//set pixel to match texture colour
			COLORREF uvColour = _model.GetTexture().GetTextureValue((int)u, (int)v);

			//code below modulates lighting into the model, is not currently implemented, but can be if comment removed
			/*float modulationR = (float)GetRValue(lightingColour) / 255;
			float modulationG = (float)GetGValue(lightingColour) / 255;
			float modulationB = (float)GetBValue(lightingColour) / 255;

			float pixelR = GetRValue(uvColour) * modulationR;
			float pixelG = GetRValue(uvColour) * modulationG;
			float pixelB = GetRValue(uvColour) * modulationB;

			COLORREF currentPixelColour = RGB(pixelR, pixelG, pixelB);*/

			SetPixel(hdc, xPos, scanlineY, uvColour);
		}

		//increment values for next pass
		currentX1 += invSlope1;
		currentX2 += invSlope2;

		cRed1 += colorSlopeRed1;
		cGreen1 += colorSlopeGreen1;
		cBlue1 += colorSlopeBlue1;

		cRed2 += colorSlopeRed2;
		cGreen2 += colorSlopeGreen2;
		cBlue2 += colorSlopeBlue2;

		cU1 += uozSlope1;
		cU2 += uozSlope2;
		cV1 += vozSlope1;
		cV2 += vozSlope2;
		cZ1 += zrSlope1;
		cZ2 += zrSlope2;
	}
	
}

void Rasteriser::TexturedFillTopFlatTriangle(const HDC& hdc, const Vertex& vertex1, const Vertex& vertex2, const Vertex& vertex3, const COLORREF& vertColour1, const COLORREF& vertColour2, const COLORREF& vertColour3)
{
	//gets change in X slope
	float invSlope1 = (float)(vertex3.GetIntX() - vertex1.GetIntX()) / (float)(vertex3.GetIntY() - vertex1.GetIntY());
	float invSlope2 = (float)(vertex3.GetIntX() - vertex2.GetIntX()) / (float)(vertex3.GetIntY() - vertex2.GetIntY());

	//gets starting X values
	float currentX1 = (float)vertex3.GetIntX();
	float currentX2 = (float)vertex3.GetIntX() + 0.5f;

	//gets colour/calculation value slopes depending on change in Y
	float v3v1Diff = (float)(vertex3.GetIntY() - vertex1.GetIntY());
	float colorSlopeBlue2 = (float)(GetBValue(vertColour3) - GetBValue(vertColour1)) / v3v1Diff;
	float colorSlopeRed2 = (float)(GetRValue(vertColour3) - GetRValue(vertColour1)) / v3v1Diff;
	float colorSlopeGreen2 = (float)(GetGValue(vertColour3) - GetGValue(vertColour1)) / v3v1Diff;
	float uozSlope2 = (float)(vertex3.GetUOZ() - vertex1.GetUOZ()) / v3v1Diff;
	float vozSlope2 = (float)(vertex3.GetVOZ() - vertex1.GetVOZ()) / v3v1Diff;
	float zrSlope2 = (float)(vertex3.GetZR() - vertex1.GetZR()) / v3v1Diff;

	float v3v2Diff = (float)(vertex3.GetIntY() - vertex2.GetIntY());
	float colorSlopeBlue1 = (float)(GetBValue(vertColour3) - GetBValue(vertColour2)) / v3v2Diff;
	float colorSlopeRed1 = (float)(GetRValue(vertColour3) - GetRValue(vertColour2)) / v3v2Diff;
	float colorSlopeGreen1 = (float)(GetGValue(vertColour3) - GetGValue(vertColour2)) / v3v2Diff;
	float uozSlope1 = (float)(vertex3.GetUOZ() - vertex2.GetUOZ()) / v3v2Diff;
	float vozSlope1 = (float)(vertex3.GetVOZ() - vertex2.GetVOZ()) / v3v2Diff;
	float zrSlope1 = (float)(vertex3.GetZR() - vertex2.GetZR()) / v3v2Diff;

	//gets starting colour/calculation values
	float cBlue1 = GetBValue(vertColour3);
	float cRed1 = GetRValue(vertColour3);
	float cGreen1 = GetGValue(vertColour3);
	float cBlue2 = GetBValue(vertColour3);
	float cRed2 = GetRValue(vertColour3);
	float cGreen2 = GetGValue(vertColour3);

	float cU1 = (float)vertex3.GetUOZ();
	float cU2 = (float)vertex3.GetUOZ();
	float cV1 = (float)vertex3.GetVOZ();
	float cV2 = (float)vertex3.GetVOZ();
	float cZ1 = (float)vertex3.GetZR();
	float cZ2 = (float)vertex3.GetZR();

	//if slopes are backwards, switch them
	if (invSlope2 < invSlope1)
	{
		float slopeTemp = invSlope1;
		invSlope1 = invSlope2;
		invSlope2 = slopeTemp;

		slopeTemp = colorSlopeRed1;
		colorSlopeRed1 = colorSlopeRed2;
		colorSlopeRed2 = slopeTemp;

		slopeTemp = colorSlopeGreen1;
		colorSlopeGreen1 = colorSlopeGreen2;
		colorSlopeGreen2 = slopeTemp;

		slopeTemp = colorSlopeBlue1;
		colorSlopeBlue1 = colorSlopeBlue2;
		colorSlopeBlue2 = slopeTemp;

		slopeTemp = uozSlope1;
		uozSlope1 = uozSlope2;
		uozSlope2 = slopeTemp;

		slopeTemp = vozSlope1;
		vozSlope1 = vozSlope2;
		vozSlope2 = slopeTemp;

		slopeTemp = zrSlope1;
		zrSlope1 = zrSlope2;
		zrSlope2 = slopeTemp;
	}

	//for each pixel on each line
	for (int scanlineY = vertex3.GetIntY(); scanlineY > vertex1.GetIntY(); scanlineY--)
	{

		for (int xPos = (int)ceil(currentX1); xPos < currentX2; xPos++)
		{
			float t = (xPos - currentX1) / (currentX2 - currentX1);

			//interpolate colour and calc values
			int red = (int)((1 - t) * cRed1 + t * cRed2);
			int green = (int)((1 - t) * cGreen1 + t * cGreen2);
			int blue = (int)((1 - t) * cBlue1 + t * cBlue2);

			float uoz = (1 - t) * cU1 + t * cU2;
			float voz = (1 - t) * cV1 + t * cV2;
			float zr = (1 - t) * cZ1 + t * cZ2;

			//convert calc values back to UV coords
			float u = uoz / zr;
			float v = voz / zr;

			COLORREF lightingColour = RGB(red, green, blue);

			//sets pixel colour to match mapped texture point
			COLORREF uvColour = _model.GetTexture().GetTextureValue((int)u, (int)v);

			//code below modulates lighting into the model, is not currently implemented but can be if comment is removed
			/*float modulationR = (float)GetRValue(lightingColour) / 255;
			float modulationG = (float)GetGValue(lightingColour) / 255;
			float modulationB = (float)GetBValue(lightingColour) / 255;

			float pixelR = GetRValue(uvColour) * modulationR;
			float pixelG = GetRValue(uvColour) * modulationG;
			float pixelB = GetRValue(uvColour) * modulationB;

			COLORREF currentPixelColour = RGB(pixelR, pixelG, pixelB);*/

			SetPixel(hdc, xPos, scanlineY, uvColour);
		}

		//decrements values for next pass
		currentX1 -= invSlope2;
		currentX2 -= invSlope1;

		cRed1 -= colorSlopeRed1;
		cGreen1 -= colorSlopeGreen1;
		cBlue1 -= colorSlopeBlue1;

		cRed2 -= colorSlopeRed2;
		cGreen2 -= colorSlopeGreen2;
		cBlue2 -= colorSlopeBlue2;

		cU1 -= uozSlope1;
		cU2 -= uozSlope2;
		cV1 -= vozSlope1;
		cV2 -= vozSlope2;
		cZ1 -= zrSlope1;
		cZ2 -= zrSlope2;
	}

}