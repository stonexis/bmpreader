#pragma once


class BMPReader {
public:
    BMPReader() : data(nullptr), width(0), height(0), bytesPerPixel(0) {}

    bool openBMP(const std::string& fileName);//метод проверки входных данных
    void displayBMP() const;//метод отображения данных на консоль
    void closeBMP(); //освобождение памяти

private:
    unsigned char* data;  // данные изображения
    int width, height;    // размеры изображения
    int bytesPerPixel;    // количество байт на пиксель
};