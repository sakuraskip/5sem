#include "os12.h"
#include <iostream>
using namespace std;
int main()
{
	try
	{
		OS12HANDEL h1 = OS12::Init();
		OS12HANDEL h2 = OS12::Init();

		cout << "adder add h1 (2,3) = " << OS12::Adder::Add(h1, 2, 3) << endl;
		cout << "adder add h2 (2,3) = " << OS12::Adder::Add(h2, 2, 3) << endl;

		cout << "adder sub h1 (2,3) = " << OS12::Adder::Sub(h1, 2, 3) << endl;
		cout << "adder sub h2 (2,3) = " << OS12::Adder::Sub(h1, 2, 3) << endl;

		cout << "multiplier multi h1 (2,3) = " << OS12::Multiplier::Mul(h1, 2, 3) << endl;
		cout << "multiplier multi h2 (2,3) = " << OS12::Multiplier::Mul(h2, 2, 3) << endl;

		cout << "multiplier div h1 (2,3) = " << OS12::Multiplier::Div(h1, 2, 3) << endl;
		cout << "multiplier div h2 (2,3) = " << OS12::Multiplier::Div(h1, 2, 3) << endl;

		OS12::Dispose(h1);
		OS12::Dispose(h2);



	}
	catch (int e)
	{
		cout << "error " << e << endl;
	}
}