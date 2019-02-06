/*
��������� ��������������� ����� ������� 24-������ BMP �� ��������� ������ 4.
*/

#include "stdafx.h"
#include <io.h>

using namespace std;

/*
@param argv[0] ����� ������ (������������ �����, ������� � 0)
@param argv[1] ��� �������� �����
@param argv[2] ��� �������������� ������� �����
@param argv[3] ��� ��������� �����
*/
int main(int argc, char *argv[]) {

	if (argc != 5)
	{
		cout << "Invalid arguments." << endl;
	}
	else
	{
		if (_access(argv[2], 0) != -1)
		{
			int n = atoi(argv[1]);
			compressedBMP in(argv[2]);
			in.compress(argv[3], n);
			in.decompress(argv[4]);
		}
		else
		{
			cout << "Input file does not exist" << endl;
		}
	}
	return 0;
}
