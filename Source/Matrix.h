#pragma once
#include "Vertex.h"

const int ROWS = 4;
const int COLS = 4;

#include <initializer_list>

class Matrix
{
public:
	// Default constructor
	Matrix();

	// Constructor that initialises all elements.
	Matrix(std::initializer_list<float> list);

	// Copy constructor
	Matrix(const Matrix& other);

	// Destructor
	~Matrix();

	// Retrieve value in matrix at specified row and column
	float GetM(const int row, const int column) const;

	// Set value in matrix at specified row and column
	void SetM(const int row, const int column, const float value);

	// Assignment operator
	Matrix& operator= (const Matrix& rhs);

	// Test for two matrices being equivalent;
	bool operator==(const Matrix& other) const;

	// Multiply two matrices together
	const Matrix operator*(const Matrix& other) const;

	// Multiply a matrix by a vertex, returning a vertex
	const Vertex operator*(const Vertex& other) const;

private:
	float _m[ROWS][COLS];

	void Copy(const Matrix& other);

};
