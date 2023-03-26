#include <iostream>         
#include <dlfcn.h>
#include <vector>

using namespace std;

int main()
{
	void* r = NULL;
    float (*SinIntegral)(float A, float B, float e);
    float (*Square)(float A, float B);
    int lib;
    cout << "0 for change libs, 1 for r1.cpp, 2 for r2.cpp, 3 for exit:\n";
    cin >> lib;
    while ((lib != 1)&&(lib != 2))
    {
        cout << "Input error, try again:\n";
        cin >> lib;
    }
    if (lib == 1)
    {
        r = dlopen("./libd1.so", RTLD_LAZY);
    }
    if (lib == 2)
    {
        r = dlopen("./libd2.so", RTLD_LAZY);
    }
    SinIntegral = (float(*)(float, float, float))dlsym(r, "SinIntegral");
    Square = (float(*)(float, float))dlsym(r, "Square");
    unsigned command;
    cout << "0 for change libs, 1 for SinIntegral, 2 for Square, 3 for exit:\n";
    cin >> command;
    while (1)
    {
        if (command == 0)
        {
            if (lib == 1)
            {
                dlclose(r);
                r = dlopen("./libd2.so", RTLD_LAZY);
                SinIntegral = (float(*)(float, float, float))dlsym(r, "SinIntegral");
                Square = (float(*)(float, float))dlsym(r, "Square");
                lib = 2;
                cout << "r1 changed on r2\n";
                cout << "0 for change libs, 1 for SinIntegral, 2 for Square, 3 for exit:\n";
                cin >> command;
                continue;
            }
            else
            {
                dlclose(r);
                r = dlopen("./libd1.so", RTLD_LAZY);
                SinIntegral = (float(*)(float, float, float))dlsym(r, "SinIntegral");
                Square = (float(*)(float, float))dlsym(r, "Square");
                lib = 1;
                cout << "r2 changed on r1\n";
                cout << "0 for change libs, 1 for SinIntegral, 2 for Square, 3 for exit:\n";
                cin >> command;
                continue;
            }
        }
        if (command == 1)
        {
            float a, b, e;
            cout << "Input A,B and step e" << '\n';
            cin >> a >> b >> e;
            cout << "Integral sin from " << a  <<" to " << b << " is " << SinIntegral(a,b,e) << '\n';
            cout << "0 for change libs, 1 for SinIntegral, 2 for Square, 3 for exit:\n";
        }
        if (command == 2)
        {
            float a,b;
            cout << "Input A,B" << '\n';
            cin >> a >> b;
            if (lib==1){
                cout << "Square of rectangle with sides " << a << " and " << b << " is " << Square(a,b) << '\n';
            }
            else{
                cout << "Square of triangle with sides " << a << " and " << b << " is " << Square(a,b) << '\n';
            }
            cout << "0 for change libs, 1 for SinIntegral, 2 for Square, 3 for exit:\n";
    
        }
        if (command == 3)
        {
            break;
        }
        cin >> command;
    }
    dlclose(r);
    return 0;
}