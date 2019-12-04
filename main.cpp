#include"HuffmanTree.h"
#include"Compress.h"
#include<string>
#include<iostream>
int main()
{
	string filename;
	cout << "Enter full your filename: ";
	cin >> filename;
	cout << "You want to:" << endl;
	cout << "1. Compress" << endl;
	cout << "2. UnCompress" << endl;
	cout << "0. Thoat" << endl;

	int i = 0;
	cin >> i;
	switch (i)
	{
	case 1:
		TestCompress(filename);
		break;
	case 2:
		TestUnCompress(filename);
		break;
	default:
		break;
	}
	system("pause");
	return 0;
}