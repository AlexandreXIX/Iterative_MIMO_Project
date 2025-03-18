// @author Alexandre P.J. Dixneuf

#ifndef PLACEHOLDER_H
#define PLACEHOLDER_H

#include <cstdint>
#include <iostream>

typedef double numType;

class Placeholder {
public:
  // Initializer + Constructor
  explicit Placeholder(const numType &input = 0);

  // Copy Constructor (banned)
  Placeholder(const Placeholder &rhs) = delete;

  // Assignment Operator (banned)
  Placeholder &operator=(const Placeholder &rhs) = delete;

  // Deconstructor (default)
  ~Placeholder() = default;

  // This is just for quick fun to re-get into C++

  // Output the stored number
  numType getNum() const;

  // Print the stored number
  void printNum() const;

  // Increase stored number by the input and print the new value
  void incNum(const numType &input);

  // Tests if two placeholders are equal
  bool isEqual(const Placeholder &rhs) const;

  // Swaps values between placeholder
  void swap(Placeholder &rhs);

  /**
   *  This mostly serves as a template for future function notes
   *  This function flips the sign of the placeholder
   *  @param N/A
   *  @return N/A
   */
  void numFlip();

  /**
   *  This mostly serves as a template for future function notes
   *  This function inverts the stored value
   *  @param N/A
   *  @return N/A
   */
  void numInvert();

private:
  // Data members
  numType myVal;
};

#endif // ITERATIVE_MIMO_PROJECT_PLACEHOLDER_H
