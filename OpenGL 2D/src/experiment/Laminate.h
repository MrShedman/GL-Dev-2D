#pragma once

#include "Matrix33.h"
#include "Material.h"
#include "Composite.h"

#include <vector>
#include <iomanip>

class Laminate
{
public:

	struct Properties
	{
		Properties(float Ex, float Ey, float Gxy, float vxy)
			:
			Ex(Ex),
			Ey(Ey),
			Gxy(Gxy),
			vxy(vxy)
		{

		}

		float Ex;
		float Ey;
		float Gxy;
		float vxy;
	};

	static Properties getProperties(Composite& comp, float plyThickness, std::vector<float> plyOrientations)
	{
		float E1 = getE1(comp);
		float E2 = getE2(comp);
		float v12 = getV12(comp);
		float G12 = getG12(comp);

		int numPlys = plyOrientations.size();

		Matrix33 Q = getQMatrix(E1, E2, v12, G12);

		std::vector<Matrix33> qBar(numPlys);

		for (int i = 0; i < numPlys; ++i)
		{
			Matrix33 theta = getThetaMatrix(plyOrientations[i]);
		
			Matrix33 psi = getPsiMatrix(plyOrientations[i]);
	
			qBar[i] = theta*Q*psi.getInverse();
		}

		Matrix33 a = getAMatrix(qBar, plyThickness).getInverse();

		float Ex = 1.0f / (plyThickness * numPlys* a[0][0]);
		float Ey = 1.0f / (plyThickness * numPlys* a[1][1]);
		float Gxy = 1.0f / (plyThickness * numPlys* a[2][2]);
		float vxy = -a[0][1] / a[0][0];

		return Properties(Ex, Ey, Gxy, vxy);
	}

private:
	static float getE1(Composite& comp)
	{
		return comp.getFibre().getModulus() * comp.getFibreVolumeFraction() +
			comp.getMatrix().getModulus() * comp.getMatrixVolumeFraction();
	}

	static float getV12(Composite& comp)
	{
		float vm = comp.getMatrix().getPoissonRatio();
		float Vm = comp.getMatrixVolumeFraction();
		float Km = comp.getMatrix().getBulk();
		float Gm = comp.getMatrix().getShear();

		float vf = comp.getFibre().getPoissonRatio();
		float Vf = comp.getFibreVolumeFraction();
		float Kf = comp.getFibre().getBulk();

		return vm*Vm + vf*Vf + ((vf - vm)*(1.0f / Km - 1.0f / Kf)*Vm*Vf) / (Vm / Kf + Vf / Km + 1.0f / Gm);
	}

	static float getG12(Composite& comp)
	{
		float Vm = comp.getMatrixVolumeFraction();
		float Gm = comp.getMatrix().getShear();

		float Vf = comp.getFibreVolumeFraction();
		float Gf = comp.getFibre().getShear();

		return Gm + (Vf / (1.0f / (Gf - Gm) + (Vm / 2.0f * Gm)));
	}

	static float getE2(Composite& comp)
	{
		float vm = comp.getMatrix().getPoissonRatio();
		float Vm = comp.getMatrixVolumeFraction();
		float Km = comp.getMatrix().getBulk();
		float Gm = comp.getMatrix().getShear();

		float vf = comp.getFibre().getPoissonRatio();
		float Vf = comp.getFibreVolumeFraction();
		float Kf = comp.getFibre().getBulk();
		float Gf = comp.getFibre().getShear();

		float kStar = Km + (Vf / (1.0f / (Kf - Km) + (Vm / (Km + Gm))));

		float Bm = 1.0f / (3.0f - 4.0f  * vm);
		float Bf = 1.0f / (3.0f - 4.0f  * vf);

		float y = Gf / Gm;

		float alpha = (Bm - y*Bf) / (1.0f + y*Bf);

		float rho = (y + Bm) / (y - 1.0f);

		float nom = (1.0f + Bm)*Vf;

		float den = rho - ((1.0f + ((3.0f  * (Bm * Bm)*(Vm *Vm)) / (alpha*(Vf * Vf * Vf) + 1.0f)))*Vf);

		float G23 = Gm * (1.0f + (nom / den));

		float v12 = getV12(comp);
		float E1 = getE1(comp);

		return 4.0f / (1.0f / G23 + 1.0f / kStar + (4.0f * (v12 * v12) / E1));
	}

	static Matrix33 getAMatrix(std::vector<Matrix33> qMats, float thickness)
	{
		float A11 = 0.0f;
		float A12 = 0.0f;
		float A16 = 0.0f;
		float A22 = 0.0f;
		float A26 = 0.0f;
		float A66 = 0.0f;

		for (size_t i = 0; i < qMats.size(); ++i)
		{
			A11 += thickness * qMats[i][0][0];
			A12 += thickness * qMats[i][0][1];
			A16 += thickness * qMats[i][0][2];
			A22 += thickness * qMats[i][1][1];
			A26 += thickness * qMats[i][1][2];
			A66 += thickness * qMats[i][2][2];
		}

			return Matrix33(A11, A12, A16,
							A12, A22, A26,
							A16, A26, A66);		
	}

	static Matrix33 getQMatrix(float E1, float E2, float v12, float G12)
	{
		float Z = (E1 - ((v12*v12)*E2)) / E1;

		float Q11 = E1 / Z;
		float Q12 = (v12*E2) / Z;
		float Q22 = E2 / Z;
		float Q66 = G12;

		return Matrix33(Q11, Q12, 0.0f,
						Q12, Q22, 0.0f,
						0.0f, 0.0f, Q66);
	}

	static Matrix33 getPsiMatrix(float angleDeg)
	{
		float m = std::cos(degToRad(angleDeg));
		float n = std::sin(degToRad(angleDeg));

		float m2 = m * m;
		float n2 = n * n;
		float mn = m*n;
		float mn2 = 2.0f * mn;
		float m2n2 = m2 - n2;

		return Matrix33(m2,	n2,	-mn,
						n2,	m2,	mn,
						mn2, -mn2, m2n2);

	}

	static Matrix33 getThetaMatrix(float angleDeg)
	{
		float m = std::cos(degToRad(angleDeg));
		float n = std::sin(degToRad(angleDeg));

		float m2 = m * m;
		float n2 = n * n;
		float mn = m*n;
		float mn2 = 2.0f * mn;
		float m2n2 = m2 - n2;

		return Matrix33(m2, n2, -mn2,
						n2, m2, mn2,
						mn, -mn, m2n2);

	}

	static float degToRad(float angleDeg)
	{
		return angleDeg * 4.0f * std::atan(1.0f) / 180.0f;
	}

private:
	Laminate(){};
};

std::ostream& operator<< (std::ostream& os, const Laminate::Properties& prop);