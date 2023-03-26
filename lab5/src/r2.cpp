#include <cmath>
extern "C" float SinIntegral(float A, float B, float e);
extern "C" float Square(float A, float B);


float SinIntegral(float A, float B, float e){
	float step_number = (B-A)/e;
	float Integral = 0;
	for (float i=0; i<step_number; i++){
		Integral+=sin(A+i*e);
	}
	Integral+=(sin(A)+sin(B))/2;
	Integral*=e;
	return Integral;
}

float Square(float A, float B){
	return A*B/2;
}