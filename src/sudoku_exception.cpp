#include <stdexcept>
#include <string>
#include "sudoku_exception.hpp"

sudoku_exception::sudoku_exception(const std::string& message) 
    : std::runtime_error(message) {}
