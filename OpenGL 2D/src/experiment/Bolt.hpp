#pragma once

#include <vector>
#include <iostream>

#include "Layer.hpp"

namespace
{
	float pi = 3.14159265358979323846f;
}

class Bolt
{
public:

	struct Properties
	{
		float preTensionCold;
		float preTensionHot;
		
		float leakageForceCold;
		float leakageForceHot;
	};

	Bolt(float youngsModulus, float yeildStrength, float UTS, float Diameter, float alpha)
		:
		E(youngsModulus),
		sigma(yeildStrength),
		UTS(UTS),
		A(pi * Diameter*Diameter*0.25f),
		L(0.f),
		alpha(alpha),
		k(0.f)
	{

	}

	void addLayer(Layer& layer)
	{
		members.push_back(layer);

		float l = 0;

		for (auto &m : members)
		{
			l += m.getLength();
		}

		setLength(l);
	}

	void setLength(float length)
	{
		L = length;
		k = (A * 1000.0f * E) / L;
	}

	float getStiffness() const
	{
		return k;
	}

	float getCombinedStiffness() const
	{
		float kInv = 0;

		for (auto &m : members)
		{
			kInv += 1.0f / m.getStiffness();
		}

		return 1.0f / kInv;
	}

	float getStiffnessRatio() const
	{
		return k / (getCombinedStiffness() + k);
	}

	Properties getProperties(float preTension, float coldTemp, float hotTemp)
	{
		float Wcold = A * preTension * sigma;

		float deltaT = hotTemp - coldTemp;

		float deltaLbexp = alpha*L*deltaT;

		float deltaLmexp = 0.0f;

		for (auto &m : members)
		{
			deltaLmexp += (m.getAlpha() * m.getLength() * deltaT);
		}

		float km = getCombinedStiffness();
	
		float Whot = (k*(deltaLbexp*km - km*deltaLmexp + Wcold) - km*Wcold) / (k - km);

		float w2Cold = -Wcold / (getStiffnessRatio() - 1.0f);

		float w2Hot = -Whot / (getStiffnessRatio() - 1.0f);

		Properties prop;
		prop.preTensionCold = Wcold / 1000.f;
		prop.preTensionHot = Whot / 1000.f;
		prop.leakageForceCold = w2Cold / 1000.f;
		prop.leakageForceHot = w2Hot / 1000.f;

		return prop;
	}

private:

	float E;
	float sigma;
	float UTS;
	float A;
	float L;
	float alpha;
	float k;

	std::vector<Layer> members;
};

std::ostream& operator << (std::ostream& os, const Bolt::Properties& p)
{
	os << "Pre-tension cold:   " << std::round(p.preTensionCold * 100.0f) / 100.0f << "kN" << std::endl;
	os << "Pre-tension hot:    " << std::round(p.preTensionHot * 100.0f) / 100.0f << "kN" << std::endl;
	os << "Leakage force cold: " << std::round(p.leakageForceCold * 100.0f) / 100.0f << "kN" << std::endl;
	os << "Leakage force hot:  " << std::round(p.leakageForceHot * 100.0f) / 100.0f << "kN" << std::endl;

	return os;
}