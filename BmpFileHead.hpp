#pragma once


class BMPReader {
public:
    BMPReader() : data(nullptr), width(0), height(0), bytesPerPixel(0) {}

    bool openBMP(const std::string& fileName);// Method to open and read a BMP file
    void displayBMP() const;// Method to display the BMP image as ASCII art (only supports black and white images).
    void closeBMP(); // Method to release memory and clean up.

private:
    unsigned char* data;  // Pointer to store the raw pixel data of the BMP image. Each pixel's color values (BGR format) are stored in this array.

    int width;  // The width of the BMP image in pixels, representing the number of horizontal pixels in each row.

    int height;  // The height of the BMP image in pixels, representing the number of vertical pixels or rows in the image.

    int bytesPerPixel;  // Number of bytes used to represent each pixel. This is typically 3 for 24-bit BMP images (BGR) or 4 for 32-bit images (BGRA).

};