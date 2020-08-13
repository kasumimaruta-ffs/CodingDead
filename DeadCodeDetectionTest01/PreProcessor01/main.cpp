#include <stdio.h>
#include <iostream>

using namespace std;

int main(void)
{

#ifdef PRE_PROCESSOR_TEST
	cout << "ifdef in" << endl;
#else
	cout << "else" << endl;
#endif

#ifdef PRE_PROCESSOR_TEST
#else
#endif

	return 0;
}