// CppConsoleApp.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <memory>
#include <windows.h>

#include "GrobalFunc.h"
#include "InlineFunction.h"
#include "ClassPolymorphic.h"
#include "CppDll.h"

//void localfunction()
//{
//	std::cout << __FUNCSIG__ << "\n";
//}

void localFunction_Z()
{
	std::cout << __FUNCSIG__ << "\n";
}

namespace local {
	void localFunction()
	{
		std::cout << __FUNCSIG__ << "\n";
	}

	void localFunction_Z()
	{
		std::cout << __FUNCSIG__ << "\n";
	}
}

namespace {
	void localFunction()
	{
		std::cout << __FUNCSIG__ << "\n";
	}

	void localFunction_Z()
	{
		std::cout << __FUNCSIG__ << "\n";
	}

	void funcPointer()
	{
		std::cout << __FUNCSIG__ << "\n";
	}

	void funcPointer_Z()
	{
		std::cout << __FUNCSIG__ << "\n";
	}
}

extern void externFunction();

namespace {
	class localclass
	{
	public:
		void memberFunc_1()
		{
			std::cout << __FUNCSIG__ << "\n";
		}

		void memberFunc_Z_1()
		{
			std::cout << __FUNCSIG__ << "\n";
		}

		void memberFunc_2()
		{
			std::cout << __FUNCSIG__ << "\n";
		}

		void memberFunc_3()
		{
			std::cout << __FUNCSIG__ << "\n";
		}

		void memberFunc_4()
		{
			std::cout << __FUNCSIG__ << "\n";
		}

		static void staticMemberFunc()
		{
			std::cout << __FUNCSIG__ << "\n";
		}

		static void staticMemberFunc_Z()
		{
			std::cout << __FUNCSIG__ << "\n";
		}

	};
}

int main()
{
	std::cout << "Hello World!\n";

	::localFunction();
	localFunction();

	local::localFunction();

	externFunction();

	grobalFunction();

	staticFunction();

	inlineFunction();

	{
		void(*funcptr)() = funcPointer;
		funcptr();
	}

	localclass().memberFunc_1();

	{
		std::unique_ptr<localclass> heaplocalclass(new localclass());
		heaplocalclass->memberFunc_2();
	}

	{
		localclass stacklocalclass;
		stacklocalclass.memberFunc_3();

		localclass& reflocalclass = stacklocalclass;
		reflocalclass.memberFunc_4();
	}

	localclass::staticMemberFunc();

	{
		std::unique_ptr<abstructclass> obj1 = classPolymorphicFactory(0);
		obj1->memFunc();
		std::unique_ptr<abstructclass> obj2 = classPolymorphicFactory(1);
		obj2->memFunc();
	}

	subclass1::staticMemberFunc();


	exportedDllFunction_1();
	exportedDllFunction_3();

	{
		HMODULE hm = LoadLibrary(L"CPPDLL.dll");
		if (hm) {
			typedef void (*VOIDFUNC)();
			VOIDFUNC funcptr1 = (VOIDFUNC)GetProcAddress(hm, "?exportedDllFunction_2@@YAXXZ");
			if (funcptr1) {
				funcptr1();
			}
			VOIDFUNC funcptr2 = (VOIDFUNC)GetProcAddress(hm, "exportedDllFunction_4");
			if (funcptr2) {
				funcptr2();
			}
			FreeLibrary(hm);
		}
	}
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します

