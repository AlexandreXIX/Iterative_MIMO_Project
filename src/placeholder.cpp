// @author Alexandre P.J. Dixneuf

#include "../include/placeholder.h"
#include <iostream>

typedef double numType;

// Initializer + Constructor
Placeholder::Placeholder(const numType& input) {myVal = input;}

// This is just for quick fun to re-get into C++

// Output the stored number
numType Placeholder::getNum() const {return myVal;}

// Print the stored number
void Placeholder::printNum() const {
    std::cout << myVal << std::endl;
}

// Increase stored number by the input and print the new value
void Placeholder::incNum(const numType& input) {
    myVal += input;
    std::cout << myVal << std::endl;
}

// Tests if two placeholders are equal
bool Placeholder::isEqual(const Placeholder &rhs) const {
    return myVal == rhs.myVal;
}

// Swaps values between placeholder
void Placeholder::swap(Placeholder &rhs) {
    numType tmp = myVal;
    myVal = rhs.myVal;
    rhs.myVal = tmp;
}

/**
 *  This mostly serves as a template for future function notes
 *  This function flips the sign of the placeholder
 *  @param N/A
 *  @return N/A
 */
void Placeholder::numFlip() {
    myVal = -myVal;
}

/**
 *  This mostly serves as a template for future function notes
 *  This function inverts the stored value
 *  @param N/A
 *  @return N/A
 */
void Placeholder::numInvert() {
    myVal = 1/myVal;
}

#include "../include/placeholder.h"
