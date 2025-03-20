// @author Alexandre P.J. Dixneuf

#include "Signal.h"

// Initializer + Constructor
Signal::Signal(const int &N_t, const int &N_r, const int &T, const int &M) : N_t(N_t), N_r(N_r), T(T), M(M), data(N_t, T) {
    // Verify that M is acceptable
    // Currently, works only on perfect squares that are powers of 2 (4, 16, 64, 256, etc.)
    int N = static_cast<int>(sqrt(M));
    if (N * N != M) {
        throw std::invalid_argument("M must be a perfect square.");
    }
    int power2Test = static_cast<int>(std::log2(M));
    if (pow(2, power2Test) != M) {
        throw std::invalid_argument("M is not a power of 2.");
    }
    // M = 2 wierd edge case and never useful, better to throw out (<= also removes negatives just in case)
    if (M <= 2) {
        throw std::invalid_argument("M is too small.");
    }
    // Initialize a random number generator
    std::random_device rd;  // a seed source for the random number engine
    std::mt19937 gen(rd()); // mersenne_twister_engine seeded with rd()
    std::uniform_int_distribution<> distrib(1, M);
    // Now for each value in the data stream, assign some random value between 0
    // and dim - 1 (dim = M in M-QAM)
    for (int i = 0; i < N_t; i++) {
        for (int j = 0; j < T; j++) {
            data(i,j) = std::complex<double>(distrib(gen) - 1, 0.0);
        }
    }
}

// This function allows a new matrix of data to replace currently stored data. This is how we update the signal after data modulation / encoding / decoding / etc.
void Signal::AlterData(const MatrixType &newMatrix) {
    data = newMatrix;
}

// This functions compare they current signal to another signal to return how accurately they match
double Signal::VerifyAccuracy(const MatrixType &variant) {
    // If matrix sizes incompatible, throw exception
    if ((data.rows() != variant.rows()) || (data.cols() != variant.cols())) {throw std::invalid_argument("Comparing matrices of different sizes.");}
    // If we assume more often accurate than not, minimally better to enter if statement only when errors occur
    double accuracy = data.size();
    for (int i = 0; i < data.rows(); i++) {
        for (int j = 0; j < data.cols(); j++) {
            if (data(i,j) != variant(i,j)) {--accuracy;}
        }
    }
    return accuracy/data.size();
}

// This function runs the verification check, but on Signal object, so must first extract data
double Signal::VerifyAccuracy(const Signal &variant) {
    return VerifyAccuracy(variant.data);
}

// Outputs a new matrix that is a deep copy of the stored data
MatrixType Signal::CopyData() { return data; }

// Outputs a pointer to the stored matrix
MatrixType& Signal::GetData() { return data; }