#include <cmath>

extern "C" float SinIntegral(float A, float B, float e);
extern "C" float Square(float A, float B);


float SinIntegral(float A, float B, float e){
	float Integral = 0;
	for (float x=A; x<=B; x+=e){
		Integral+=sin(x-e/2)*e;
	}
	return Integral;
}

float Square(float A, float B){
	return A * B;
}
