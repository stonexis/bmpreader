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

    // Читаем заголовок файла
    BITMAPFILEHEADER fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));

    // Проверяем, что это BMP файл
    if (fileHeader.bfType != 0x4D42) {
        std::cerr << "Error: Not a BMP file!" << std::endl;
        return false;
    }

    // Читаем заголовок информации
    BITMAPINFOHEADER infoHeader;
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));

    // Поддерживаем только 24 или 32-битные BMP файлы
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
        std::cerr << "Error: Only 24 or 32-bit BMP files are supported." << std::endl;
        return false;
    }

    // Сохраняем размеры изображения и количество байт на пиксель
    width = infoHeader.biWidth;
    height = infoHeader.biHeight;
    bytesPerPixel = infoHeader.biBitCount / 8;

    // Вычисляем выравнивание строки в BMP файлах (строки выравниваются до кратности 4 байтам)
    int padding = (4 - (width * bytesPerPixel) % 4) % 4;

    // Выделяем память для данных изображения
    data = new unsigned char[(width * bytesPerPixel + padding) * height];

    // Переходим к началу данных пикселей
    file.seekg(fileHeader.bfOffBits, std::ios::beg);

    // Чтение данных пикселей построчно (снизу вверх)
    for (int i = 0; i < height; ++i) {
        file.read(reinterpret_cast<char*>(data + i * (width * bytesPerPixel + padding)), width * bytesPerPixel + padding);
        //file.read(reinterpret_cast<char*>(data + i * (width * bytesPerPixel + padding)), width * bytesPerPixel);
        //file.ignore(padding);  // Пропускаем padding
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
    // Выводим изображение построчно (снизу вверх)
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            unsigned char* pixel = data + y * (width * bytesPerPixel + padding) + x * bytesPerPixel;

            // Читаем данные в формате BGR
            unsigned char red = pixel[2];
            unsigned char green = pixel[1];
            unsigned char blue = pixel[0];

            // Черный пиксель
            if (red <= COLOR_THRESHOLD && green <= COLOR_THRESHOLD && blue <= COLOR_THRESHOLD) {
                std::cout << '#';
            }
            // Белый пиксель
            else if (red >= 255 - COLOR_THRESHOLD && green >= 255 - COLOR_THRESHOLD && blue >= 255 - COLOR_THRESHOLD) {
                std::cout << ' ';
            }
            // Если другой цвет, выводим ошибку
            else {
                std::cerr << "Error: Unsupported color found. Only black and white images are allowed." << std::endl;
                std::cerr << "Unsupported color: R=" << static_cast<int>(red)
                    << " G=" << static_cast<int>(green)
                    << " B=" << static_cast<int>(blue) << std::endl;
                return;
            }
        }
       
        std::cout << std::endl;  // Переход на новую строку после вывода строки пикселей
    }
}

void BMPReader::closeBMP() {
    delete[] data;
    data = nullptr;
    width = height = bytesPerPixel = 0;
}


