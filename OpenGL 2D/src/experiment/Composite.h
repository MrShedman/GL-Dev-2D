#pragma once

#include "Material.h"

class Composite
{
public:
	Composite(Material& fibre, Material& matrix, float Vf) : fibre(fibre), matrix(matrix), Vf(Vf), Vm(1.0f - Vf)
	{}

	Material& getFibre()
	{
		return fibre;
	}

	Material& getMatrix()
	{
		return matrix;
	}

	float getFibreVolumeFraction() const
	{
		return Vf;
	}

	float getMatrixVolumeFraction() const
	{
		return Vm;
	}

private:

	float Vf;
	float Vm;
	Material& fibre;
	Material& matrix;
};