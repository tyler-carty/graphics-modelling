#include "Model.h"
#include <algorithm>
#include <math.h>
#include <wingdi.h>


Model::Model()
{

	/*
	sets the reflection coefficients for the lighting on the modelspo
	*/

	_ka[0] = { 0.2f };
	_ka[1] = { 0.2f };
	_ka[2] = { 0.2f };

	_kd[0] = { 0.5f };
	_kd[1] = { 0.5f };
	_kd[2] = { 0.5f };

	_ks[0] = { 1.0f };
	_ks[1] = { 1.0f };
	_ks[2] = { 1.0f };
}

Model::~Model()
{

}

//returns the polygon list
const std::vector<Polygon3D>& Model::GetPolygons()
{
	return _polygons;
}

//returns the default vertex list
const std::vector<Vertex>& Model::GetVertices()
{
	return _originalVertices;
}

//returns the transformed vertex list
const std::vector<Vertex>& Model::GetTransVertices()
{
	return _transformedVertices;
}

//returns the UV coord list
const std::vector<UVCoord>& Model::GetUVCoords()
{
	return _uvCoordinates;
}

//counts the number of polygons
size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

//counts the number of vertices
size_t Model::GetVertexCount() const
{
	return _originalVertices.size();
}

//returns the texture that is loaded
Texture& Model::GetTexture()
{
	return _texture;
}

//adds vertex to vertex list for the model
void Model::AddVertex(float x, float y, float z)
{
	_originalVertices.push_back(Vertex(x, y, z, 1));
}

//adds polygon to the polygon list for the model
void Model::AddPolygon(int i0, int i1, int i2, int uvIndex0, int uvIndex1, int uvIndex2)
{
	_polygons.push_back(Polygon3D(i0, i1, i2, uvIndex0, uvIndex1, uvIndex2));
}

//adds possible UV coordinate to list
void Model::AddTextureUV(float u, float v)
{
	_uvCoordinates.push_back(UVCoord(u, v));
}

void Model::ApplyTransformToLocalVertices(const Matrix& transform)
{
	//loop through original vertices, apply matrix and write to transformed list
	// transVertex = origVertex * transform

	_transformedVertices.clear();

	for (int i = 0; i < _originalVertices.size(); i++)
	{
		_transformedVertices.push_back(transform * _originalVertices[i]);
	}

}

void Model::ApplyTransformToTransformedVertices(const Matrix& transform)
{
	//loop through transformed vertices, applying the matrix and update each index in that list
	// transVertex = transVertex * transform

	for (int i = 0; i < _originalVertices.size(); i++)
	{
		_transformedVertices[i] = transform * _transformedVertices[i];
	}

}

//dehomogenizes the vertex coordinates
void Model::Dehomogenized()
{
	for (int i = 0; i < _originalVertices.size(); i++)
	{
		_transformedVertices[i].SetPreTranZ(_transformedVertices[i].GetW());

		_transformedVertices[i].Dehomogenized();
	}

}

void Model::CalculateBackfaces(Camera _camera)
{

	for (int i = 0; i < _polygons.size(); i++)
	{

		// Get the vertices for those indices 

		Vertex vertex0 = _transformedVertices[_polygons[i].GetIndex(0)];
		Vertex vertex1 = _transformedVertices[_polygons[i].GetIndex(1)];
		Vertex vertex2 = _transformedVertices[_polygons[i].GetIndex(2)];

		//Construct vector a by subtracting vertex 1 from vertex 0.

		Vector3D vectorA = vertex0 - vertex1;

		//Construct vector b by subtracting vertex 2 from vertex 0. 

		Vector3D vectorB = vertex0 - vertex2;

		//Calculate the normal vector from vector b and a 
		Vector3D vectorNormal = Vector3D::CreateCrossProduct(vectorA, vectorB);

		//Create eye - vector = vertex 0 - camera position
		//Vector3D vectorEye = Vertex::CalculateVector(vertex0, _camera.GetCameraPosition());

		Vector3D vectorEye(vertex0 - _camera.GetCameraPosition());

		//Take dot product of the normal and eye-vector
		float dotProduct = Vector3D::CreateDotProduct(vectorNormal, vectorEye);

		//If result < 0
		//Mark the polygon for culling
		if (dotProduct > 0.0f) {
			_polygons[i].SetCullState(true);
		}
		else {
			_polygons[i].SetCullState(false);
		}

		_polygons[i].SetPolygonNormal(vectorNormal);

	}

}


//loops through each polygon, sets the average Z value for each polygon and then sorts the collection by AVG Z
void Model::Sort(void)
{
	for (int i = 0; i < _polygons.size(); i++) 
	{

		Vertex vertex0 = _transformedVertices[_polygons[i].GetIndex(0)];
		Vertex vertex1 = _transformedVertices[_polygons[i].GetIndex(1)];
		Vertex vertex2 = _transformedVertices[_polygons[i].GetIndex(2)];

		float averagePolygonZ = (vertex0.GetZ() + vertex1.GetZ() + vertex2.GetZ()) / 3;

		_polygons[i].SetAverageZ(averagePolygonZ);
	}

	sort(_polygons.begin(), _polygons.end(), sortByAvgZ);

}

//Binary Predicate for sorting by AVG Z
bool Model::sortByAvgZ(const Polygon3D& lhs, const Polygon3D& rhs)
{
	return lhs.GetAverageZ() > rhs.GetAverageZ();
}

//loops through polygons to calculate the lighting effect from ambient lighting
void Model::CalculateLightingAmbient(const AmbientLighting& ambientLight)
{

	for (int i = 0; i < _polygons.size(); i++)
	{

		float totalR = 0;
		float totalG = 0;
		float totalB = 0;

		totalR += (ambientLight.GetRedValue() * _ka[0]);
		totalG += (ambientLight.GetGreenValue() * _ka[1]);
		totalB += (ambientLight.GetBlueValue() * _ka[2]);

		_polygons[i].SetRGBValue(RGB(totalR, totalG, totalB));

	}

}

//loops through polygons to calculate the lighting effect from directional lighting
void Model::CalculateLightingDirectional(const std::vector<DirectionalLighting>& lightingVectors)
{

	float totalR;
	float totalG;
	float totalB;

	float tempR;
	float tempG;
	float tempB;

	for (int i = 0; i < _polygons.size(); i++)
	{

		totalR = GetRValue(_polygons[i].GetRGBValue());
		totalG = GetGValue(_polygons[i].GetRGBValue());;
		totalB = GetBValue(_polygons[i].GetRGBValue());;

		for (int j = 0; j < lightingVectors.size(); j++)
		{

			Vector3D normalisedLightingVector = Vector3D::NormaliseVector(lightingVectors[j].GetLightDirectionVector());
			Vector3D normalisedPolygonNormalVector = Vector3D::NormaliseVector(_polygons[i].GetPolygonNormal());

			tempR = (lightingVectors[j].GetRedValue()) * _kd[0];
			tempG = (lightingVectors[j].GetGreenValue()) * _kd[1];
			tempB = (lightingVectors[j].GetBlueValue()) * _kd[2];

			float dotProduct = Vector3D::CreateDotProduct(normalisedLightingVector, normalisedPolygonNormalVector);


			tempR = tempR * dotProduct;
			tempG = tempG * dotProduct;
			tempB = tempB * dotProduct;

			totalR = totalR + tempR;
			totalG = totalG + tempG;
			totalB = totalB + tempB;

		}

		totalR = DirectionalLighting::ClampRGBValues(totalR);
		totalG = DirectionalLighting::ClampRGBValues(totalG);
		totalB = DirectionalLighting::ClampRGBValues(totalB);

		_polygons[i].SetRGBValue(RGB(int(totalR), int(totalG), int(totalB)));

	}

}

//loops through polygons to calculate the lighting effect from point lighting
void Model::CalculateLightingPoint(const std::vector<PointLighting>& pointLights)
{
	for (int i = 0; i < _polygons.size(); i++)
	{

		float totalR = GetRValue(_polygons[i].GetRGBValue());
		float totalG = GetGValue(_polygons[i].GetRGBValue());
		float totalB = GetBValue(_polygons[i].GetRGBValue());

		for (int j = 0; j < pointLights.size(); j++)
		{

			float a = pointLights[j].GetValueA();
			float b = pointLights[j].GetValueB();
			float c = pointLights[j].GetValueC();

			Vertex vertex0 = _transformedVertices[_polygons[i].GetIndex(0)];
			Vertex pointPosition = pointLights[j].GetPointPosition();

			Vector3D vectorToPointLight = vertex0 - pointPosition;

			Vector3D polygonNormalVector = _polygons[i].GetPolygonNormal();

			//calculate angle of polygon normal with light point.
			float angle = acos(Vector3D::CreateDotProduct(vectorToPointLight, polygonNormalVector) / (Vector3D::CalculateMagnitude(vectorToPointLight) * Vector3D::CalculateMagnitude(polygonNormalVector)));

			if (angle >= 0) {

				float d = Vector3D::CalculateMagnitude(vectorToPointLight);

				float attenuation = 1 / (a + (b * d) + (c * d * d));

				float tempR = float(pointLights[j].GetRedValue());
				float tempG = float(pointLights[j].GetGreenValue());
				float tempB = float(pointLights[j].GetBlueValue());

				tempR = 20 * _kd[0] * tempR * angle * attenuation;
				tempG = 20 * _kd[0] * tempG * angle * attenuation;
				tempB = 20 * _kd[0] * tempB * angle * attenuation;

				totalR += tempR;
				totalG += tempG;
				totalB += tempB;

			}
		}

		totalR = DirectionalLighting::ClampRGBValues(totalR);
		totalG = DirectionalLighting::ClampRGBValues(totalG);
		totalB = DirectionalLighting::ClampRGBValues(totalB);

		_polygons[i].SetRGBValue(RGB(int(totalR), int(totalG), int(totalB)));

	}
}

//loops through each vertex to calculate the lighting effect from ambient lighting
void Model::CalculateVertexLightingAmbient(const AmbientLighting& ambientLight)
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{

		float totalR = 0;
		float totalG = 0;
		float totalB = 0;

		totalR += (ambientLight.GetRedValue() * _ka[0]);
		totalG += (ambientLight.GetGreenValue() * _ka[1]);
		totalB += (ambientLight.GetBlueValue() * _ka[2]);

		_transformedVertices[i].SetVertexRGB(RGB(totalR, totalG, totalB));
	}
}

//loops through each vertex to calculate the lighting effect from directional lighting
void Model::CalculateVertexLightingDirectional(const std::vector<DirectionalLighting>& lightingVectors)
{
	float totalR;
	float totalG;
	float totalB;

	float tempR;
	float tempG;
	float tempB;

	for (int i = 0; i < _transformedVertices.size(); i++)
	{

		totalR = GetRValue(_transformedVertices[i].GetVertexRGB());
		totalG = GetGValue(_transformedVertices[i].GetVertexRGB());;
		totalB = GetBValue(_transformedVertices[i].GetVertexRGB());;

		for (int j = 0; j < lightingVectors.size(); j++)
		{

			Vector3D normalisedLightingVector = Vector3D::NormaliseVector(lightingVectors[j].GetLightDirectionVector());
			Vector3D normalisedVertexNormalVector = Vector3D::NormaliseVector(_transformedVertices[i].GetVertexNormal());

			tempR = (lightingVectors[j].GetRedValue()) * _kd[0];
			tempG = (lightingVectors[j].GetGreenValue()) * _kd[1];
			tempB = (lightingVectors[j].GetBlueValue()) * _kd[2];

			float dotProduct = Vector3D::CreateDotProduct(normalisedLightingVector, normalisedVertexNormalVector);

			tempR = tempR * dotProduct;
			tempG = tempG * dotProduct;
			tempB = tempB * dotProduct;

			totalR = totalR + tempR;
			totalG = totalG + tempG;
			totalB = totalB + tempB;

		}

		totalR = DirectionalLighting::ClampRGBValues(totalR);
		totalG = DirectionalLighting::ClampRGBValues(totalG);
		totalB = DirectionalLighting::ClampRGBValues(totalB);

		_transformedVertices[i].SetVertexRGB(RGB(int(totalR), int(totalG), int(totalB)));

	}
}

//loops through each vertex to calculate the lighting effect from point lighting
void Model::CalculateVertexLightingPoint(const std::vector<PointLighting>& pointLights)
{
	for (int i = 0; i < _transformedVertices.size(); i++)
	{

		float totalR = GetRValue(_transformedVertices[i].GetVertexRGB());
		float totalG = GetGValue(_transformedVertices[i].GetVertexRGB());
		float totalB = GetBValue(_transformedVertices[i].GetVertexRGB());

		for (int j = 0; j < pointLights.size(); j++)
		{

			float a = pointLights[j].GetValueA();
			float b = pointLights[j].GetValueB();
			float c = pointLights[j].GetValueC();

			Vertex vertex0 = _transformedVertices[i];
			Vertex pointPosition = pointLights[j].GetPointPosition();

			Vector3D vectorToPointLight = vertex0 - pointPosition;

			Vector3D vertexNormalVector = _transformedVertices[i].GetVertexNormal();

			//calculate angle of polygon normal with light point.
			float angle = acos(Vector3D::CreateDotProduct(vectorToPointLight, vertexNormalVector) / (Vector3D::CalculateMagnitude(vectorToPointLight) * Vector3D::CalculateMagnitude(vertexNormalVector)));

			if (angle >= 0) {

				float d = Vector3D::CalculateMagnitude(vectorToPointLight);

				float attenuation = 1 / (a + (b * d) + (c * d * d));

				float tempR = float(pointLights[j].GetRedValue());
				float tempG = float(pointLights[j].GetGreenValue());
				float tempB = float(pointLights[j].GetBlueValue());

				tempR = 20 * _kd[0] * tempR * angle * attenuation;
				tempG = 20 * _kd[0] * tempG * angle * attenuation;
				tempB = 20 * _kd[0] * tempB * angle * attenuation;

				totalR += tempR;
				totalG += tempG;
				totalB += tempB;

			}
		}

		totalR = DirectionalLighting::ClampRGBValues(totalR);
		totalG = DirectionalLighting::ClampRGBValues(totalG);
		totalB = DirectionalLighting::ClampRGBValues(totalB);

		_transformedVertices[i].SetVertexRGB(RGB(int(totalR), int(totalG), int(totalB)));

	}
}

//loops through each polygon and vertex to make sure that the 
//correct vertex normals are found according to how many vertices contribute to that area of space

void Model::CalculateVertexNormal()
{

	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		_transformedVertices[i].SetVertexNormal(Vector3D(0, 0, 0));
		_transformedVertices[i].SetContributeCount(0);
	}

	for (int i = 0; i < _polygons.size(); i++)
	{
		for (int j = 0; j <= 2; j++)
		{
			Vector3D currentPolygonNormal = Vector3D::NormaliseVector(_polygons[i].GetPolygonNormal());

			Vector3D currentVertexNormal = _transformedVertices[_polygons[i].GetIndex(j)].GetVertexNormal();
			_transformedVertices[_polygons[i].GetIndex(j)].SetVertexNormal(currentVertexNormal + currentPolygonNormal);

			int count = _transformedVertices[_polygons[i].GetIndex(j)].GetContributeCount();
			_transformedVertices[_polygons[i].GetIndex(j)].SetContributeCount(count += 1);
		}
	}

	for (int i = 0; i < _transformedVertices.size(); i++)
	{
		
		Vector3D returnNormal = _transformedVertices[i].GetVertexNormal() / _transformedVertices[i].GetContributeCount();
		_transformedVertices[i].SetVertexNormal(Vector3D::NormaliseVector(returnNormal));
	}

}
