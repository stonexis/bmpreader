#pragma once


class BMPReader {
public:
    BMPReader() : data(nullptr), width(0), height(0), bytesPerPixel(0) {}

    bool openBMP(const std::string& fileName);//����� �������� ������� ������
    void displayBMP() const;//����� ����������� ������ �� �������
    void closeBMP(); //������������ ������

private:
    unsigned char* data;  // ������ �����������
    int width, height;    // ������� �����������
    int bytesPerPixel;    // ���������� ���� �� �������
};