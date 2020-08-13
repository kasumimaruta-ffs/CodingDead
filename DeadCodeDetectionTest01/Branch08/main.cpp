#include <iostream>

using namespace std;

void Print(int num)
{
	cout << "Print : " << num << "‰ñ–Ú" << endl;
}

int main(void)
{
	int num = 0;

	do {
		num++;
		Print(num);
	} while (false);

	do {
		num++;
		Print(num);
	} while (100 == num);

	return 0;
}