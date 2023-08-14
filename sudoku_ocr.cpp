#include <cstdio>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <iostream>
#include <string>
#include <format>
#include <filesystem>
#include <cctype>


const std::string PATH { "cells" };



unsigned count_files_in_directory(const std::string& path) {
    const std::filesystem::path dir_path{ path };
    unsigned i {};
    for (auto const& dir_entry : std::filesystem::directory_iterator{dir_path}) {
        ++i;
    }
    return i;
}

int main() {
    unsigned sudoku_size = count_files_in_directory(PATH);
    unsigned n {1};
  
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(".", "eng")) {
        perror("Could not initialize tesseract\n");
        exit(1);
    }

    Pix *image;
    for (unsigned i {1}; i <= sudoku_size; ++i) {
        std::string filename = std::format("{}/Cell_{}.png", PATH, i);
        image = pixRead(filename.c_str());
        api->SetImage(image);
        std::string outText = api->GetUTF8Text();
        if (outText.size() >= 1 && std::isdigit(outText[0])) {
            std::cout << outText[0] << " ";
        } else {
            std::cout << "0" << " ";
        }
        pixDestroy(&image);

        if (i % 9 == 0) {
            std::cout << "\n";
        }
    }




    // FREEING RESOURCES
    api->End();
    delete api;
    //delete[] outText;
    //pixDestroy(&image);
    return 0;
}