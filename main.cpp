/*
Программа преобразовывает файлы формата 24-битный BMP по алгоритму Добеши 4.
*/

#include "stdafx.h"
#include <io.h>

using namespace std;

/*
@param argv[0] порог сжатия (вещественное число, близкое к 0)
@param argv[1] имя входного файла
@param argv[2] имя промежуточного сжатого файла
@param argv[3] имя итогового файла
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
