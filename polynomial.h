#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

#include <string>

class PolynomialImpl; // forward declaration

class Polynomial {
private:
    PolynomialImpl* pImpl; // hidden data structure
public:
    Polynomial();
    ~Polynomial();

    void insertTerm(int coefficient, int exponent);
    std::string toString() const;
    Polynomial add(const Polynomial& other) const;
    Polynomial multiply(const Polynomial& other) const;
    Polynomial derivative() const;
};

#endif
