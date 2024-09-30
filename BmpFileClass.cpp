#include <iostream>
#include <fstream>
#include <windows.h>
#include "BmpFileHead.hpp"

bool BMPReader::openBMP(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << fileName << std::endl;
        return false;
    }

    // ������ ��������� �����
    BITMAPFILEHEADER fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));

    // ���������, ��� ��� BMP ����
    if (fileHeader.bfType != 0x4D42) {
        std::cerr << "Error: Not a BMP file!" << std::endl;
        return false;
    }

    // ������ ��������� ����������
    BITMAPINFOHEADER infoHeader;
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

    // ������������ ������ 24 ��� 32-������ BMP �����
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
        std::cerr << "Error: Only 24 or 32-bit BMP files are supported." << std::endl;
        return false;
    }

    // ��������� ������� ����������� � ���������� ���� �� �������
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    bytesPerPixel = infoHeader.biBitCount / 8;

    // ��������� ������������ ������ � BMP ������ (������ ������������� �� ��������� 4 ������)
    int padding = (4 - (width * bytesPerPixel) % 4) % 4;

    // �������� ������ ��� ������ �����������
    data = new unsigned char[(width * bytesPerPixel + padding) * height];

    // ��������� � ������ ������ ��������
    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    // ������ ������ �������� ��������� (����� �����)
    for (int i = 0; i < height; ++i) {
        file.read(reinterpret_cast<char*>(data + i * (width * bytesPerPixel + padding)), width * bytesPerPixel + padding);
        //file.read(reinterpret_cast<char*>(data + i * (width * bytesPerPixel + padding)), width * bytesPerPixel);
        //file.ignore(padding);  // ���������� padding
    }

    file.close();
    return true;
}

void BMPReader::displayBMP() const {
    if (!data) {
        std::cerr << "Error: No image data to display." << std::endl;
        return;
    }
    const int COLOR_THRESHOLD = 10;
    int padding = (4 - (width * bytesPerPixel) % 4) % 4;
    // ������� ����������� ��������� (����� �����)
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            unsigned char* pixel = data + y * (width * bytesPerPixel + padding) + x * bytesPerPixel;

            // ������ ������ � ������� BGR
            unsigned char red = pixel[2];
            unsigned char green = pixel[1];
            unsigned char blue = pixel[0];

            // ������ �������
            if (red <= COLOR_THRESHOLD && green <= COLOR_THRESHOLD && blue <= COLOR_THRESHOLD) {
                std::cout << '#';
            }
            // ����� �������
            else if (red >= 255 - COLOR_THRESHOLD && green >= 255 - COLOR_THRESHOLD && blue >= 255 - COLOR_THRESHOLD) {
                std::cout << ' ';
            }
            // ���� ������ ����, ������� ������
            else {
                std::cerr << "Error: Unsupported color found. Only black and white images are allowed." << std::endl;
                std::cerr << "Unsupported color: R=" << static_cast<int>(red)
                    << " G=" << static_cast<int>(green)
                    << " B=" << static_cast<int>(blue) << std::endl;
                return;
            }
        }
       
        std::cout << std::endl;  // ������� �� ����� ������ ����� ������ ������ ��������
    }
}

void BMPReader::closeBMP() {
    delete[] data;
    data = nullptr;
    width = height = bytesPerPixel = 0;
}


