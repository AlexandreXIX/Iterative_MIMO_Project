// @author Alexandre P.J. Dixneuf

#ifndef MYSIGNAL_H
#define MYSIGNAL_H

#include <Eigen/Dense>
#include <complex>
#include <variant>
#include <random>
#include <iostream>

/**
 * @class MySignal
 * @brief This class serves as a container for a signal.
 * @details To avoid asking a user to understand the Eigen class and how to handle matrices, it is better to create this class. This class should generate random signals and handle the matrix automatically (again, simplifies for user.) All other classes for this project will simply take in the Object rather than each handle random matrices and run exception tests at each step.
 *          NOTE: THE CLASS CANNOT BE CALLED "Signal", because that is in the standard library and will destroy everything.
 * @param This container needs four values, N_t, N_r, T, and M
 *        N_t: The number of transceiver antennas
 *        N_r: The number of receiving antennas
 *        T: The number of transmissions / the timespan of signal transmissions
 *        M: The modulation level. Wished it wasn't necessary for this container, but if 256-QAM, message can't be >256
 */

// Defines a matrix as being dynamically sized with complex double values
using MatrixType = Eigen::Matrix<std::complex<double>, Eigen::Dynamic, Eigen::Dynamic>;

class MySignal {
public:
    // Initializer + Constructor
    MySignal(const int &N_t, const int &N_r, const int &T, const int &M);

    // Copy Constructor (Deep-Copy) can be default, Eigen does deep copy by default
    MySignal(const MySignal &rhs) = default;

    // Assignment Operator (Deep-Copy) can be default, Eigen does deep copy by default
    MySignal &operator=(const MySignal &rhs) = default;

    // Destructor (default)
    ~MySignal() = default;

    // This function allows a new matrix of data to replace currently stored data. This is how we update the signal after data modulation / encoding / decoding / etc.
    void AlterData(const MatrixType &newMatrix);

    // This functions compare they current signal to another signal to return how accurately they match
    double VerifyAccuracy(const MatrixType &variant);

    // This function runs the verification check, but on Signal object, so must first extract data
    double VerifyAccuracy(const MySignal &variant);

    // Outputs a new matrix that is a deep copy of the stored data
    MatrixType CopyData();

    // Outputs a pointer to the stored matrix
    MatrixType& GetData();

private:
    int N_t;
    int N_r;
    int T;
    int M;
    MatrixType data;
};

#endif //MYSIGNAL_H
