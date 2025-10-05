#include "polynomial.h"
#include <map>
#include <sstream>
#include <string>
#include <algorithm>

class PolynomialImpl {
public:
    // Using map to store exponent → coefficient (sorted automatically by exponent)
    std::map<int, int, std::greater<int>> terms;
};

Polynomial::Polynomial() : pImpl(new PolynomialImpl()) {}

Polynomial::~Polynomial() { delete pImpl; }

void Polynomial::insertTerm(int coefficient, int exponent) {
    if (exponent < 0) return; // Ignore invalid exponent
    if (coefficient == 0) return; // Ignore zero terms

    pImpl->terms[exponent] += coefficient;

    // Remove if term becomes zero after combining
    if (pImpl->terms[exponent] == 0) {
        pImpl->terms.erase(exponent);
    }
}

std::string Polynomial::toString() const {
    if (pImpl->terms.empty()) return "0";

    std::ostringstream out;
    bool firstTerm = true;

    for (const auto& term : pImpl->terms) {
        int coeff = term.second;
        int exp = term.first;

        if (coeff == 0) continue;

        // Handle sign
        if (!firstTerm) {
            out << (coeff > 0 ? "+" : "-");
        } else {
            if (coeff < 0) out << "-";
            firstTerm = false;
        }

        // Handle absolute coefficient (omit 1 if exp > 0)
        int absCoeff = std::abs(coeff);
        if (absCoeff != 1 || exp == 0)
            out << absCoeff;

        // Handle variable part
        if (exp > 0) {
            out << "x";
            if (exp > 1)
                out << "^" << exp;
        }
    }

    return out.str();
}

Polynomial Polynomial::add(const Polynomial& other) const {
    Polynomial result;

    // Add terms from this polynomial
    for (const auto& term : pImpl->terms) {
        result.insertTerm(term.second, term.first);
    }

    // Add terms from other polynomial
    for (const auto& term : other.pImpl->terms) {
        result.insertTerm(term.second, term.first);
    }

    return result;
}

Polynomial Polynomial::multiply(const Polynomial& other) const {
    Polynomial result;

    for (const auto& [exp1, coeff1] : pImpl->terms) {
        for (const auto& [exp2, coeff2] : other.pImpl->terms) {
            int newCoeff = coeff1 * coeff2;
            int newExp = exp1 + exp2;
            result.insertTerm(newCoeff, newExp);
        }
    }

    return result;
}

Polynomial Polynomial::derivative() const {
    Polynomial result;

    for (const auto& [exp, coeff] : pImpl->terms) {
        if (exp > 0)
            result.insertTerm(coeff * exp, exp - 1);
    }

    return result;
}
