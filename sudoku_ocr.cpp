#include <leptonica/allheaders.h>
#include <tesseract/baseapi.h>
#include <iostream>
#include <string>
#include <format>
#include <sys/types.h> // DELETE
#include <dirent.h> // DELETE
#include <filesystem>
#include <algorithm>

const std::string PATH { "cells" };



unsigned count_files_in_directory(const std::string& path) {
    const std::filesystem::path dir_path{ path };
    unsigned i {};
    for (auto const& dir_entry : std::filesystem::directory_iterator{dir_path}) {
        std::cout << dir_entry.path() << "\n";
        ++i;
    }
    return i;
}

int main() {
    std::cout << count_files_in_directory(PATH) << std::endl;
    PIX *pixs, *pixd;
   

    return 0;
}