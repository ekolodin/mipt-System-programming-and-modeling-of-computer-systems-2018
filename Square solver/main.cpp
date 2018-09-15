/*! \file */
#include <iostream>
#include <cmath>
#include <random>

#define WARN(assertion) std::cout << ((assertion) ? "Pass successful\n\n" : "Wrong answer to this test\n\n");

const double eps = 1e-9;
const int INF_ROOTS = -1;

inline bool is_zero(double number) {
    return fabs(number) <= eps;
}

int solve_linear(double b, double c, double *x1) {
    if (is_zero(b)) {
        return (c == 0 ? INF_ROOTS : 0);
    } else {
        *x1 = -c / b;
        return 1;
    }
}

/*! Solves a square equation ax^2 + bx + c = 0
 *
 *  \param [in] a   a ‐ double coefficient
 *  \param [in] b   b ‐ double coefficient
 *  \param [in] c   c ‐ double coefficient
 *  \param [out] x1 Pointer to the 1st root
 *  \param [out] x2 Pointer to the 2nd root
 *
 *  \return Number of roots
 *
 *  \note In case of infinite number of roots,
 *  returns INF_ROOTS.
 */

int solve_square(double a, double b, double c, double *x1, double *x2) {

    assert (std::isfinite(a));
    assert (std::isfinite(b));
    assert (std::isfinite(c));

    assert(x1 != nullptr);
    assert(x2 != nullptr);
    assert(x1 != x2);

    if (is_zero(a)) {
        return solve_linear(b, c, x1);
    } else {
        double d = b * b - 4 * a * c;
        if (is_zero(d)) {
            *x1 = *x2 = -b / (2 * a);
            return 1;
        } else if (d < 0) {
            return 0;
        } else {
            double sqrt_d = sqrt(d);
            *x1 = (-b - sqrt_d) / (2 * a);
            *x2 = (-b + sqrt_d) / (2 * a);
            return 2;
        }
    }
}

class Solve_square_test {
public:

    void all_zeroes_test() {
        std::cout << "All zeroes test\n";
        WARN(solve_square(0, 0, 0, &x1, &x2) == -1);
    }

    void multi_random_test(int number) {
        for (int i = 1; i <= number; ++i) {
            random_test(i);
        }
    }

    void one_root_test() {
        std::cout << "One root test\n";
        WARN(solve_square(1, 2, 1, &x1, &x2) == 1 && (x1 == -1));
    }

    void two_roots_test() {
        std::cout << "Two roots test\n";
        WARN((solve_square(1, 10, 1, &x1, &x2) == 2) &&
             (is_zero(x1 * x1 + 10 * x1 + 1)) &&
             (is_zero(x2 * x2 + 10 * x2 + 1)));
    }

private:
    void random_test(int number) {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen(rd()); // Standard mersenne_twister_engine seeded with rd()
        std::uniform_real_distribution<> dis(-1000, 1000);

        double a = dis(gen);
        double b = dis(gen);
        double c = dis(gen);
        std::cout << "Random test №" << number << ": a = " << a << ", b = " << b << ", c = " << c << "\n";

        int nRoots = solve_square(a, b, c, &x1, &x2);
        switch (nRoots) {
            case 0:
                WARN(b * b - 4 * a * c < 0);
                break;
            case 1:
                if (a) {
                    WARN(b * b - 4 * a * c == 0 && is_zero(a * x1 * x1 + b * x1 + c));
                } else {
                    WARN(is_zero(a * x1 * x1 + b * x1 + c));
                }
                break;
            case 2:
                WARN((b * b - 4 * a * c > 0) &&
                     is_zero(a * x1 * x1 + b * x1 + c) &&
                     is_zero(a * x2 * x2 + b * x2 + c));
                break;
            case -1:
                x1 = dis(gen);
                WARN(is_zero(a * x1 * x1 + b * x1 + c));
                break;
            default:
                std::cout << "Couldn't pass random test\n";
                WARN(false);
                break;
        }
    }

private:
    double x1 = 0, x2 = 0;
};

int main() {
    std::cout << "Square equation solver\n";
    Solve_square_test solve_square_test;
    solve_square_test.all_zeroes_test();
    solve_square_test.multi_random_test(10);
    solve_square_test.one_root_test();
    solve_square_test.two_roots_test();
    std::cout << "All tests pass successfully\n";
    std::cout << "Enter a, b, c:\n";

    double a = 0, b = 0, c = 0, x1 = 0, x2 = 0;
    std::cin >> a >> b >> c;

    int n_roots = solve_square(a, b, c, &x1, &x2);

    switch (n_roots) {
        case 0:
            std::cout << "No roots\n";
            break;
        case 1:
            std::cout << "x = " << x1;
            break;
        case 2:
            std::cout << "x1 = " << x1 << ", x2 = " << x2;
            break;
        case -1:
            std::cout << "Any number";
            break;
        default:
            std::cout << "main(): ERROR: nRoots = " << n_roots;
            return 1;
    }

    return 0;
}