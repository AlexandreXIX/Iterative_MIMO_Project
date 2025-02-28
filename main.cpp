// @author Alexandre P.J. Dixneuf
#include <iostream>
#include "src/placeholder.cpp"
typedef double numType;

int main() {
    // Quick testing
    numType FirstVal = 4;
    std::cout << "Let a be a 4" << std::endl;
    Placeholder a(FirstVal);
    std::cout << "Am I storing a 4?" << std::endl;
    numType tester = a.getNum();
    std::cout << "My tester says: " << tester << std::endl;
    std::cout << "What happens if I change either the initial val or the tester? Can it still automatically print the 4?" << std::endl;
    tester++;
    FirstVal = 10;
    a.printNum();
    std::cout << "Now we increase it by 0.8 to get 4.8" << std::endl;
    numType NewVal = 0.8;
    a.incNum(NewVal);
    std::cout << "Let b be 4.8 and c be -10:" << std::endl;
    Placeholder b(4.8);
    Placeholder c(-10);
    std::cout << "a and b should be equal and return True" << std::endl;
    std::cout << a.isEqual(b) << std::endl;
    std::cout << "a and c should not be equal and return False" << std::endl;
    std::cout << a.isEqual(c) << std::endl;
    std::cout << "Swapping b and c should inverse these results" << std::endl;
    b.swap(c);
    std::cout << a.isEqual(b) << std::endl;
    std::cout << a.isEqual(c) << std::endl;
    std::cout << "Flipping the sign of a should output -4.8" << std::endl;
    a.numFlip();
    a.printNum();
    std::cout << "Inverting a (after the flip) should output approximately -0.2" << std::endl;
    a.numInvert();
    a.printNum();
}
