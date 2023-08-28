#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>
#include <memory>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <unordered_map>
#include "sudoku.hpp"
#include "sudoku_exception.hpp"


bool Sudoku::sudoku_values_in_range() const noexcept  { 
    for (const std::vector<unsigned>& row : sdk) {
        for (unsigned x : row) {
            if (!(x == EMPTY || (x >= MIN && x <= MAX))) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::sudoku_completed() const noexcept {
    for (const std::vector<unsigned>& row : sdk) {
        for (unsigned x : row) {
            if (x == EMPTY) {
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::unique_values_in_row(const std::vector<unsigned>& row) const noexcept{
      if (!row.size()) {
         return false;
     }
     std::unordered_map<unsigned, unsigned> ltb;
     for (unsigned x : row) {
         if (x == EMPTY) {
             continue;
         }
         if (ltb.find(x) != ltb.end()) {
             return false;
         }
         ltb[x] = 1;
     }
     return true;
}


bool Sudoku::unique_values_in_col(const Matrix& mt, unsigned nth_col) const noexcept {
    std::unordered_map<unsigned, unsigned> ltb;
    for (unsigned int i {}; i < N; ++i) {
        if (mt[i][nth_col] == EMPTY) {
            continue;
        }
        if (ltb.find(mt[i][nth_col]) != ltb.end()) {
            return false;
        }
        ltb[mt[i][nth_col]] = 1;
    }
    return true;
}


bool Sudoku::unique_values_in_region(const Matrix& mt, unsigned row, unsigned col) const noexcept{
    unsigned start_row { (row / REGION_SIZE) * REGION_SIZE };
    unsigned start_col { (col / REGION_SIZE) * REGION_SIZE };
    unsigned end_row { start_row + REGION_SIZE };
    unsigned end_col { start_col + REGION_SIZE };
    std::unordered_map<unsigned, unsigned> ltb;
    for (unsigned int i { start_row }; i < end_row; ++i) {
        for (unsigned int j { start_col }; j < end_col; j = ++j) {
            if (mt[i][j] == EMPTY) {
                continue;
            }
            if (ltb.find(mt[i][j]) != ltb.end()) {
                return false;
            }
            ltb[mt[i][j]] = 1;
            }
        }
    return true;
}


std::optional<std::tuple<unsigned, unsigned>> Sudoku::find_next_hole() const {
    for (unsigned int i {  }; i < N; ++i) {
        for (unsigned int j {  }; j < N; ++j) {
            if (sdk[i][j] == EMPTY) {
                return std::make_tuple(i, j);
            }
        }
    }
    return std::nullopt;
}


Sudoku::Sudoku(Matrix& contents) : N(contents.size()){
    if (!sudoku_values_in_range()) {
        throw std::runtime_error("Sudoku values are not valid");
    }
    sdk = contents;
}


bool Sudoku::check_if_valid()  {
    for (const auto& row : sdk) {
        if (!unique_values_in_row(row)) {
            return false;
        }
    }
    
    for (unsigned int nth_col {}; nth_col < N; ++nth_col) {
        if (!unique_values_in_col(sdk, nth_col)) {
            return false;
        }
    }
    for (unsigned int i {}; i < N; i += REGION_SIZE) {
        for (unsigned int j {}; j < N; j += REGION_SIZE) {
            if (!unique_values_in_region(sdk, i, j)) {
                return false;
            }
        }
    }
    return true;
}


void Sudoku::backtrack(unsigned n, unsigned current_row, unsigned current_col) {
    if (std::all_of(sdk.begin(), sdk.end(), 
    [&](const auto& v) { return std::all_of(v.begin(), v.end(), [&](auto x) { return x != EMPTY; }); })) {
        throw sudoku_exception("sudoku finished");
    }
    sdk[current_row][current_col] = n;
    if (!(unique_values_in_row(sdk[current_row]) && unique_values_in_col(sdk, current_col) && unique_values_in_region(sdk, current_row, current_col))) {
        sdk[current_row][current_col] = EMPTY;
        return;
    }
    
    auto next_hole { find_next_hole() };
    if (next_hole == std::nullopt) {
        return;
    }        
    const auto [next_row, next_col] = next_hole.value() ;
    
    for (unsigned int i {1}; i <= N; ++i) {
        backtrack(i, next_row, next_col);
    }
    sdk[current_row][current_col] = EMPTY;
}


void Sudoku::solve() {
    auto next_hole { find_next_hole() };
    if (next_hole == std::nullopt) {
        return;
    }
    
    try {
    for (unsigned int i {1}; i <= N; ++i) {
        backtrack(i, std::get<0>(next_hole.value()), std::get<1>(next_hole.value()));
    }
    } catch(sudoku_exception& s) { }
}


const Sudoku::Matrix& Sudoku::get_matrix() {
    return sdk;
}



