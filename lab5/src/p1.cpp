#include <iostream>

extern "C" float SinIntegral(float A, float B, float e);
extern "C" float Square(float A, float B);

using namespace std;

int main(){
	int command;
	cout << "Enter 1 for Integral calculation, 2 for square calculation, 3 for exit" << '\n';
	cin >> command;
	while (1){
		if (command==1){
			float A,B,e;
			cout << "Input A,B and step e" << '\n';
			cin >> A >> B >> e;
			cout << "Integral sin from " << A  <<" to " << B << " is " << SinIntegral(A,B,e) << '\n';
			cout << "Enter 1 for Integral calculation, 2 for square calculation, 3 for exit" << '\n';
		}
		if (command==2){
			float A,B;
			cout << "Input A,B" << '\n';
			cin >> A >> B;
			cout << "Square of rectangle with sides " << A << " and " << B << " is " << Square(A,B) << '\n';
			cout << "Enter 1 for Integral calculation, 2 for square calculation, 3 for exit" << '\n';
		}
		if (command == 3)
        {
            break;
        }
        cin >> command;
	}
	return 0;
}