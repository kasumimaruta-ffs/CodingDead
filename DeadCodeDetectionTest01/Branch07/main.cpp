#include <iostream>

using namespace std;

enum Week { Mon, Tue, Wed, Thu, Fri, Sat, Sun };

Week GetDayOfTheWeek(void)
{
	return Week::Sun;
}

int main(void)
{
	Week today = GetDayOfTheWeek();

	switch (today)
	{
	case Mon:
		cout << "出勤1日目" << endl;
		break;
	case Tue:
		cout << "出勤2日目" << endl;
		break;
	case Wed:
		cout << "出勤3日目" << endl;
		break;
	case Thu:
		cout << "出勤4日目" << endl;
		break;
	case Fri:
		cout << "出勤5日目" << endl;
		break;
	case Sat:
		cout << "休日1日目" << endl;
		break;
	case Sun:
		cout << "休日2日目" << endl;
		break;
	default:
		break;
	}

	return 0;
}