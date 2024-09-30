#include <iostream>
#include <fstream>
#include <windows.h>
#include "BmpFileHead.hpp"

// Method to open and read a BMP file
bool BMPReader::openBMP(const std::string& fileName) {
    std::ifstream file(fileName, std::ios::binary);  // Opens the BMP file in binary mode for reading.
    if (!file.is_open()) {  // Checks if the file was successfully opened.
        std::cerr << "Error: Could not open file " << fileName << std::endl;  // Prints an error message if the file cannot be opened.
        return false;
    }

    // Reads the file header (BITMAPFILEHEADER).
    BITMAPFILEHEADER fileHeader;
    file.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));  // Reads the file header from the BMP file into fileHeader.

    // Checks if the file is a BMP file (0x4D42 represents the ASCII characters 'BM' for BMP).
    if (fileHeader.bfType != 0x4D42) {
        std::cerr << "Error: Not a BMP file!" << std::endl;  // Prints an error message if the file is not a valid BMP.
        return false;
    }

    // Reads the BMP info header (BITMAPINFOHEADER).
    BITMAPINFOHEADER infoHeader;
    file.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));  // Reads the info header from the BMP file into infoHeader.

    // Checks if the BMP file is either 24-bit or 32-bit.
    if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
        std::cerr << "Error: Only 24 or 32-bit BMP files are supported." << std::endl;  // Prints an error message if the file is not 24 or 32 bits.
        return false;
    }

    // Stores the width, height, and bytes per pixel of the image.
    width = infoHeader.biWidth;  // Sets the width of the BMP image.
    height = infoHeader.biHeight;  // Sets the height of the BMP image.
    bytesPerPixel = infoHeader.biBitCount / 8;  // Calculates how many bytes per pixel (3 for 24-bit, 4 for 32-bit).

    // Calculates the row padding (BMP rows are aligned to multiples of 4 bytes).
    int padding = (4 - (width * bytesPerPixel) % 4) % 4;  // Calculates how many padding bytes are added to the end of each row.

    // Allocates memory for the pixel data.
    data = new unsigned char[(width * bytesPerPixel + padding) * height];  // Allocates memory to store the entire image (including padding).

    // Moves the file pointer to the beginning of the pixel data.
    file.seekg(fileHeader.bfOffBits, std::ios::beg);  // Positions the file reader at the beginning of the pixel data using the offset.

    // Reads the pixel data row by row (bottom to top).
    for (int i = 0; i < height; ++i) {
        file.read(reinterpret_cast<char*>(data + i * (width * bytesPerPixel + padding)), width * bytesPerPixel + padding);  // Reads each row of pixel data (including padding).
    }

    file.close();  // Closes the file after reading.
    return true;
}

// Method to display the BMP image as ASCII art (only supports black and white images).
void BMPReader::displayBMP() const {
    if (!data) {  // Checks if image data is available.
        std::cerr << "Error: No image data to display." << std::endl;  // Prints an error message if no data exists.
        return;
    }
    const int COLOR_THRESHOLD = 10;  // Sets a threshold to determine if a color is considered black or white.
    int padding = (4 - (width * bytesPerPixel) % 4) % 4;  // Recalculates the padding (BMP rows are aligned to 4-byte boundaries).

    // Displays the image row by row (bottom to top).
    for (int y = height - 1; y >= 0; --y) {  // Iterates from the last row (bottom) to the first row (top).
        for (int x = 0; x < width; ++x) {  // Iterates over each pixel in the row.
            unsigned char* pixel = data + y * (width * bytesPerPixel + padding) + x * bytesPerPixel;  // Gets the address of the current pixel.

            // Reads pixel data in BGR order (BMP stores colors in Blue, Green, Red format).
            unsigned char red = pixel[2];  // Reads the red component from the pixel.
            unsigned char green = pixel[1];  // Reads the green component from the pixel.
            unsigned char blue = pixel[0];  // Reads the blue component from the pixel.

            // Checks if the pixel is black.
            if (red <= COLOR_THRESHOLD && green <= COLOR_THRESHOLD && blue <= COLOR_THRESHOLD) {
                std::cout << '#';  // Prints '#' for black pixels.
            }
            // Checks if the pixel is white.
            else if (red >= 255 - COLOR_THRESHOLD && green >= 255 - COLOR_THRESHOLD && blue >= 255 - COLOR_THRESHOLD) {
                std::cout << ' ';  // Prints a space for white pixels.
            }
            // If the color is not black or white, it prints an error message.
            else {
                std::cerr << "Error: Unsupported color found. Only black and white images are allowed." << std::endl;  // Prints an error for unsupported colors.
                std::cerr << "Unsupported color: R=" << static_cast<int>(red)
                    << " G=" << static_cast<int>(green)
                    << " B=" << static_cast<int>(blue) << std::endl;  // Displays the RGB values of the unsupported pixel.
                return;
            }
        }
        std::cout << std::endl;  // Moves to the next line after each row of pixels is printed.
    }
}

// Method to release memory and clean up.
void BMPReader::closeBMP() {
    delete[] data;  // Frees the allocated memory for image data.
    data = nullptr;  // Resets the pointer to avoid dangling pointers.
    width = height = bytesPerPixel = 0;  // Resets the width, height, and bytesPerPixel to 0 (cleanup).
}



