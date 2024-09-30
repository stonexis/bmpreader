#include <iostream>
#include <fstream>
#include <windows.h>
#include "BmpFileHead.hpp"
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: drawBmp.exe <path_to_bmp_file>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    BMPReader bmp;

    // ��������� BMP ����
    if (!bmp.openBMP(filePath)) {
        return 1;
    }

    // ���������� BMP ����
    bmp.displayBMP();

    // ����������� �������
    bmp.closeBMP();

    return 0;
}