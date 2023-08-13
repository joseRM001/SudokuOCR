#include <array>
#include <cmath>
#include <optional>
template <unsigned N>
class Sudoku {
    using Matrix = std::array<std::array<unsigned, N>, N>;
    Matrix sdk;
    const unsigned REGION_SIZE = std::sqrt(N);
    static constexpr unsigned MIN { 1 };
    static constexpr unsigned MAX { N };
    static constexpr unsigned EMPTY { 0 };

    static bool value_in_range(unsigned x) noexcept;
    static bool sudoku_values_in_range(const Matrix& sudoku) noexcept;
    bool unique_values_in_row(const std::array<unsigned, N>& row) const noexcept;
    bool unique_values_in_col(const Matrix& mt, unsigned nth_col) const noexcept;
    bool unique_values_in_region(const Matrix& mt, unsigned row, unsigned col) const noexcept;
    std::optional<std::tuple<unsigned, unsigned>> find_next_hole() const;
    void backtrack(unsigned n, unsigned current_row, unsigned current_col);

    public:
    Sudoku(Matrix& contents);
    const Matrix& get_matrix();
    bool check_if_valid();
    void solve();
};