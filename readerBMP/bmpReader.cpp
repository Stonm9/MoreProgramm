// Viewer b\w BMP files for Windows, test task Petrov Denis 2024
#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>

class ReaderBMP {
    std::string fileName_;
    BITMAPFILEHEADER fileHeader = {};
    BITMAPINFOHEADER infoHeader = {};
    std::vector<unsigned char> pixelData_;
    int width_ = 0;
    int height_ = 0;
    int bitCount_ = 0;
    int rowSize_ = 0;
    std::ifstream file_;

public:
    explicit ReaderBMP(const std::string& fileName) : fileName_(fileName) {
        openBMP(fileName_);
        displayBMP();
        closeBMP();
    };
private:
    void openBMP(const std::string& fileName) {
        file_.open(fileName_, std::ios::binary);
        if (!file_.is_open()) {
            std::cerr << "Error open file: " << fileName << "\n";
            return;
        }

        // Читаем заголовок
        file_.read(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
        if (fileHeader.bfType != 0x4D42) {
            std::cerr << "This is not BMP file: " << fileName_ << "\n";
            return;
        }

        // Читаем заголовок информации
        file_.read(reinterpret_cast<char*>(&infoHeader), sizeof(BITMAPINFOHEADER));
        if (infoHeader.biBitCount != 24 && infoHeader.biBitCount != 32) {
            std::cerr << "File is not 24 or 32 bit BMP: " << fileName_ << "\n";
            return;
        }

        width_ = infoHeader.biWidth;
        height_ = infoHeader.biHeight;
        bitCount_ = infoHeader.biBitCount;
        rowSize_ = ((bitCount_ * width_ + 31) / 32) * 4; // Размер строки с выравниванием

        // Чтение данных пикселей
        pixelData_.resize(rowSize_ * height_);
        file_.seekg(fileHeader.bfOffBits, std::ios::beg);
        file_.read(reinterpret_cast<char*>(pixelData_.data()), rowSize_ * height_);
    }
    void displayBMP() {
        if (pixelData_.empty()) {
            std::cerr << "No image data to display.\n";
            return;
        }
        std::cout << "Width: " << width_ << "\n";
        std::cout << "Height: " << height_ << "\n";
        std::cout << "Bits: " << bitCount_ << "\n";
        // Отображение изображения на экране
        for (int y = height_ - 1; y >= 0; --y) {  
            for (int x = 0; x < width_; ++x) {
                int pixelOffset = y * rowSize_ + x * (bitCount_ / 8);
                unsigned char blue = pixelData_[pixelOffset];
                unsigned char green = pixelData_[pixelOffset + 1];
                unsigned char red = pixelData_[pixelOffset + 2];

                // Проверка, если это черный или белый пиксель
                if (red == 255 && green == 255 && blue == 255) {
                    std::cout << " ";  
                }
                else if (red == 0 && green == 0 && blue == 0) {
                    std::cout << "#"; 
                }
            }
            std::cout << std::endl;
        }
    }
    void closeBMP() {
        // Закрываем файл, очищаем вектор
        if (file_.is_open()) {
            file_.close();  
        }
        pixelData_.clear();  
    }
};
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " path_to_bmp_file\n";
        return 1;
    }
    std::string fileName = argv[1];
    ReaderBMP myBMP(fileName);
    //ReaderBMP myBMP24("Star24.bmp");
    //ReaderBMP myBMP32("Star32.bmp");
    return 0;
}

