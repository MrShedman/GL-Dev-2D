#pragma once

class Material
{
public:

	Material(float modulus, float poissonRatio)
	{
		E = modulus;
		v = poissonRatio;

		setShear(getShearModuli(*this));
		setBulk(getBulkModuli(*this));
	}

	void setModulus(float modulus)
	{
		E = modulus;
	}

	void setPoissonRatio(float poissonRatio)
	{
		v = poissonRatio;
	}

	void setShear(float shear)
	{
		G = shear;
	}

	void setBulk(float bulk)
	{
		K = bulk;
	}

	float getModulus() const
	{
		return E;
	}

	float getPoissonRatio() const
	{
		return v;
	}

	float getShear() const
	{
		return G;
	}

	float getBulk() const
	{
		return K;
	}

private:

	float getShearModuli(Material& mat)
	{
		return mat.getModulus() / (2.0f*(1.0f + mat.getPoissonRatio()));
	}

	float getBulkModuli(Material& mat)
	{
		return mat.getModulus() / (3.0f*(1.0f - 2.0f * mat.getPoissonRatio()));
	}

	float E;
	float v;
	float G;
	float K;
};