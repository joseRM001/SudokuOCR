#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <iostream>
#include <string>
#include <format>
#include <filesystem>
#include <cctype>
#include <vector>
#include "sudoku.hpp"

const std::string PATH { "cells" };



unsigned count_files_in_directory(const std::string& path) {
    const std::filesystem::path dir_path{ path };
    unsigned i {};
    for (auto const& dir_entry : std::filesystem::directory_iterator{dir_path}) {
        ++i;
    }
    return i;
}

bool is_number(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](char c) { return std::isdigit(c); });
}

int main() {
    const unsigned sudoku_size = count_files_in_directory(PATH);
    const unsigned side_length = std::sqrt(sudoku_size) ;
    unsigned n {1};
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();
    if (api->Init(".", "eng")) {
        perror("Could not initialize tesseract\n");
        exit(1);
    }

    std::vector<std::vector<unsigned>> mt;
    mt.reserve(side_length);

    Pix *image;
    unsigned i {1};
    std::vector<unsigned> row;
    row.reserve(side_length);
        
    while (n <= side_length) {
        std::string filename = std::format("{}/Cell_{}.png", PATH, i);
        image = pixRead(filename.c_str());
        api->SetImage(image);
        std::string outText = api->GetUTF8Text();
        // Tesseract adds a '\n' character at the end of the detected word
        if (outText.size() >= 1 && is_number(outText.substr(0, outText.size() - 1))) {
            row.push_back(std::stoi(outText.substr(0, outText.size() - 1)));
        } else {
            row.push_back(0);
        }
        pixDestroy(&image);

        i += side_length;
        if (i > sudoku_size) {
            mt.emplace_back(row);
            row.clear();
            i = ++n;
        }
       
    }

    for (const std::vector<unsigned>& row : mt) {
        for (unsigned n : row) {
            std::cout << n << " ";
        }
        std::cout << "\n";
    }
    // FREEING RESOURCES
    api->End();
    delete api;
    return 0;
}