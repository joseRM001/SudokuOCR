#pragma once
#include <string>
#include <stdexcept>

class sudoku_exception : public std::runtime_error {
    public:
    sudoku_exception(const std::string& message);
};