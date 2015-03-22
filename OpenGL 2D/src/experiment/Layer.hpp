#pragma once

class Layer
{
public:

	Layer(float E, float A, float L, float alpha)
		:
		E(E),
		A(A),
		L(L),
		alpha(alpha),
		k((A*1000.0f*E) / L)
	{

	}

	float getStiffness() const
	{
		return k;
	}

	float getLength() const
	{
		return L;
	}

	float getAlpha() const
	{
		return alpha;
	}

private:

	float E;
	float A;
	float L;
	float alpha;
	float k;
};