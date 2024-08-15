#include <iostream>
using namespace std;
#include "mysdk.h"

int main()
{
	cout << "Hello Server." << MySDK::getVersion() << endl;
	return 0;
}