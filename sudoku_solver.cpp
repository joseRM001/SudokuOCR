#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <array>
#include <memory>
#include <optional>
#include <stdexcept>
#include <tuple>
#include <unordered_map>

template <unsigned N>
class Sudoku {
    using Matrix = std::array<std::array<unsigned, N>,N>;
    Matrix pMt;
    unsigned int region_size = std::sqrt(N);
    static constexpr unsigned int MIN { 1 };
    static constexpr unsigned int MAX { N };
    static constexpr unsigned int EMPTY { 0 };

    static bool value_in_range(unsigned x) { return (x == EMPTY) || (x >= MIN && x <= MAX); }
    static bool sudoku_values_in_range(const Matrix& sudoku) { 
        return std::all_of(sudoku.begin(), sudoku.end(), [](std::array<unsigned, N> row) { return std::all_of(row.begin(), row.end(), value_in_range); }); 
    }

    bool unique_values_in_row(const std::array<unsigned, N>& row) const {
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

    bool unique_values_in_col(const Matrix& mt, unsigned nth_col) const {
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

    bool unique_values_in_region(const Matrix& mt, unsigned row, unsigned col) const {
        unsigned start_row { (row / region_size) * region_size };
        unsigned start_col { (col / region_size) * region_size };
        unsigned end_row { start_row + region_size };
        unsigned end_col { start_col + region_size };
        std::unordered_map<unsigned, unsigned> ltb;
        for (unsigned int i { start_row }; i < end_row; ++i) {
            for (unsigned int j { start_col }; j < end_col; j = ++j) {
                //std::cout << mt[i][j] << " ";
                if (mt[i][j] == EMPTY) {
                    continue;
                }
                if (ltb.find(mt[i][j]) != ltb.end()) {
                    return false;
                }
                ltb[mt[i][j]] = 1;
            }
            //std::cout << "\n";
        }
        return true;
    }

    std::optional<std::tuple<unsigned, unsigned>> find_next_hole() {
        for (unsigned int i {  }; i < N; ++i) {
            for (unsigned int j {  }; j < N; ++j) {
                if (pMt[i][j] == EMPTY) {
                    return std::make_tuple(i, j);
                }
            }
        }
        return std::nullopt;
    }

    public:
    Sudoku(Matrix& contents) {
        if (!sudoku_values_in_range(contents)) {
            throw std::runtime_error("Sudoku values are not valid");
        }
        pMt = contents;
    }

    bool check_if_valid()  {
        for (const auto& row : pMt) {
            if (!unique_values_in_row(row)) {
                return false;
            }
        }
        
        for (unsigned int nth_col {}; nth_col < N; ++nth_col) {
            if (!unique_values_in_col(pMt, nth_col)) {
                return false;
            }
        }

        for (unsigned int i {}; i < N; i += region_size) {
            for (unsigned int j {}; j < N; j += region_size) {
                if (!unique_values_in_region(pMt, i, j)) {
                    return false;
                }
            }
        }

        return true;
    }



    void backtrack(unsigned n, unsigned current_row, unsigned current_col) {
        if (std::all_of(pMt.begin(), pMt.end(), 
        [](const auto& v) { return std::all_of(v.begin(), v.end(), [](auto x) { return x != EMPTY; }); })) {
            throw std::runtime_error("uwu");
        }

        pMt[current_row][current_col] = n;
        if (!check_if_valid()) {
            pMt[current_row][current_col] = EMPTY;
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
        pMt[current_row][current_col] = EMPTY;
    }

    void solve() {
        auto next_hole { find_next_hole() };
        if (next_hole == std::nullopt) {
            return;
        }
        
        try {
        for (unsigned int i {1}; i <= N; ++i) {
            backtrack(i, std::get<0>(next_hole.value()), std::get<1>(next_hole.value()));
        }
        } catch(...) {

        }
    }

    const Matrix& get_matrix() {
        return pMt;
    }
};


int main() {
    const unsigned int n {9};
    std::array<std::array<unsigned, n>, n> mt {{
        {{5, 3, 0, 0, 7, 0, 0, 0, 0}},
        {{6, 0, 0, 1, 9, 5, 0, 0, 0}},
        {{0, 9, 8, 0, 0, 0, 0, 6, 0}},
        {{8, 0, 0, 0, 6, 0, 0, 0, 3}},
        {{4, 0, 0, 8, 0, 3, 0, 0, 1}},
        {{7, 0, 0, 0, 2, 0, 0, 0, 6}},
        {{0, 6, 0, 0, 0, 0, 2, 8, 0}},
        {{0, 0, 0, 4, 1, 9, 0, 0, 5}},
        {{0, 0, 0, 0, 8, 0, 0, 7, 9}}
    }};

    std::array<std::array<unsigned, 16>, 16> mt1 {{
        {{0, 0, 0, 11, 6, 0, 12, 13, 0, 1, 3, 0, 14, 0, 2, 0}},
        {{2, 0, 15, 10, 9, 16, 0, 14, 4, 8, 0, 12, 0, 0, 13, 5}},
        {{9, 14, 0, 0, 11, 0, 0, 0, 0, 0, 6, 0, 3, 8, 15, 12}},
        {{0, 6, 1, 0, 0, 0, 10, 8, 0, 0, 5, 0, 0, 16, 0, 9}},
        {{6, 0, 0, 15, 4, 5, 2, 12, 16, 0, 13, 0, 8, 11, 0, 14}},
        {{7, 0, 9, 2, 10, 0, 0, 0, 12, 0, 0, 15, 0, 1, 6, 3}},
        {{0, 11, 3, 5, 14, 0, 15, 0, 8, 10,1, 7, 4, 0, 12, 13}},
        {{13, 0, 12, 14, 0, 0, 7, 3, 11, 5, 0, 6, 16, 0, 10, 15}},
        {{12, 15, 0, 16, 3, 0, 4, 1,9, 7, 0, 0, 6, 5, 0, 10}},
        {{8,3, 0, 9, 7, 10,5, 15, 0, 6, 0, 11, 13, 12, 1, 0}},
        {{1,5, 7, 0, 13, 0, 0, 11, 0, 0, 0, 10, 2, 15, 0, 4}},
        {{10, 0, 14, 13,0, 12, 0, 6, 5, 4, 15, 1, 9, 0, 0, 11}},
        {{4, 0, 5, 0, 0, 9, 0, 0, 1, 15, 0, 0, 0, 14, 11, 0}},
        {{14, 12, 16, 1, 0, 13, 0, 0, 0, 0, 0, 8, 0, 0, 9, 2}},
        {{11, 9, 0, 0, 15, 0, 14, 4, 13, 0, 12, 5, 10, 3, 0, 1}},
        {{0, 10, 0, 8, 0, 3, 16, 0, 14, 11, 0, 4, 12, 0, 0}}
    }};

    Sudoku<16> sdk(mt1);
    sdk.solve();
    auto solved = sdk.get_matrix();

    for (unsigned int i {}; i < 16; ++i) {
        for (unsigned int j {}; j < 16; ++j) {
            std::cout << std::setw(3)<<solved[i][j];
        }
        std::cout << "\n";
    }
    std::cout << "Sudoku valid ? : " << sdk.check_if_valid() << "\n"; 

    return 0;
}