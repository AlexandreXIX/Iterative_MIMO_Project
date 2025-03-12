// @author Alexandre P.J. Dixneuf
#include "src/placeholder.cpp"
// #include "src/System.cpp"
#include <iostream>
#include <vector>
#include <random>
#include <Eigen/Dense>

typedef double numType;
using namespace std;
using namespace Eigen;

const int Nt = 4; // Number of transmit antennas
const int Nr = 4; // Number of receive antennas
const int Iterations = 10; // Number of BP iterations
const double NoiseVariance = 0.1; // Noise variance

// Function to generate a random channel matrix with Rayleigh fading
MatrixXd generateChannel(int nr, int nt) {
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> dist(0.0, 1.0);
    MatrixXd H(nr, nt);
    for (int i = 0; i < nr; ++i)
        for (int j = 0; j < nt; ++j)
            H(i, j) = dist(gen) / sqrt(2.0);
    return H;
}

// Function to generate transmitted BPSK symbols (+1 or -1)
VectorXd generateBPSK(int nt) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dist(0, 1);
    VectorXd x(nt);
    for (int i = 0; i < nt; ++i)
        x(i) = dist(gen) ? 1.0 : -1.0;
    return x;
}

// Function to add Gaussian noise to the received signal
VectorXd addNoise(const VectorXd& y, double noiseVar) {
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<double> noise(0.0, sqrt(noiseVar));
    VectorXd noisyY = y;
    for (int i = 0; i < y.size(); ++i)
        noisyY(i) += noise(gen);
    return noisyY;
}

// Belief Propagation decoding for MIMO
VectorXd beliefPropagation(const MatrixXd& H, const VectorXd& y) {
    int nt = H.cols();
    VectorXd x_hat = VectorXd::Zero(nt);
    VectorXd LLR = H.transpose() * y; // Initial LLR

    for (int iter = 0; iter < Iterations; ++iter) {
        for (int i = 0; i < nt; ++i) {
            double sum = 0;
            for (int j = 0; j < H.rows(); ++j) {
                sum += H(j, i) * (y(j) - H.row(j) * x_hat + H(j, i) * x_hat(i));
            }
            LLR(i) = sum / NoiseVariance;
        }
        for (int i = 0; i < nt; ++i)
            x_hat(i) = (LLR(i) > 0) ? 1.0 : -1.0;
    }
    return x_hat;
}

int main() {
    // Placeholder Matrix Test

    // Generate MIMO channel
    MatrixXd H = generateChannel(Nr, Nt);

    // Generate transmitted symbols
    VectorXd x = generateBPSK(Nt);

    // Received signal: y = Hx + noise
    VectorXd y = H * x;
    y = addNoise(y, NoiseVariance);

    // BP decoding
    VectorXd x_decoded = beliefPropagation(H, y);

    // Output results
    cout << "Transmitted symbols: " << x.transpose() << endl;
    cout << "Received symbols: " << y.transpose() << endl;
    cout << "Decoded symbols:     " << x_decoded.transpose() << endl;

    // Leaving Old Placeholder for now
    // Quick testing
    numType FirstVal = 4;
    std::cout << "Let a be a 4" << std::endl;
    Placeholder a(FirstVal);
    std::cout << "Am I storing a 4?" << std::endl;
    numType tester = a.getNum();
    std::cout << "My tester says: " << tester << std::endl;
    std::cout << "What happens if I change either the initial val or the tester? "
                 "Can it still automatically print the 4?"
              << std::endl;
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
    std::cout << "Inverting a (after the flip) should output approximately -0.2"
              << std::endl;
    a.numInvert();
    a.printNum();
    return 0;
}
