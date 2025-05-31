// @author Alexandre P.J. Dixneuf

#ifndef DECODER_H
#define DECODER_H

#include "Channel.h"
#include "QAMConstellation.h"
#include <Eigen/Dense>

/**
 * @class Decoder
 * @brief A virtual function to serve as a template for future decoders
 * @details This sets the requirements for a decoder function to work with the
 * rest of the code. It must be capable of handling a ProblemParameters object,
 * to get the necessary problem daa. In addition, it must take in and output
 * Eigen::MatrixXcd as the input and output data. Lastly, it handles the
 * QAMConstellation class to be able to convert int to complex and complex to
 * int
 * @param ProblemParameters object, QAMConstellation object
 */

class Decoder {
public:
  Decoder(const Eigen::MatrixXcd &Y, const ProblemParameters *params,
          const QAMConstellation &myQAM, const Channel &myChannel)
      : data(Y), myParams(params), constellation(myQAM.GetMapInt2Complex()),
        H(myChannel.GetH()), Z(myChannel.GetZ()) {}
  virtual ~Decoder() = default;

  Eigen::MatrixXcd Run() {
    decode();
    complexToSymbol();
    return data;
  }

  virtual void decode() = 0; // Pure virtual function

  // Values must be public for override classes to access, doesn't matter since
  // class can't be an object, only thing a user can do is call "Run()"
  Eigen::MatrixXcd data;
  const ProblemParameters *myParams;
  std::unordered_map<int, std::complex<double>> constellation;
  Eigen::MatrixXcd H;
  Eigen::MatrixXcd Z;

private:
  void complexToSymbol() {
    Eigen::MatrixXcd symbols = data;
    for (int i = 0; i < data.rows(); i++) {
      for (int j = 0; j < data.cols(); j++) {
        std::complex<double> current = data(i, j);
        std::complex<double> bestSymbol(999, 999);
        double smallestDifference = std::numeric_limits<double>::max();
        for (int testIdx = 0; testIdx < myParams->GetM(); testIdx++) {
          std::complex<double> tmp = constellation.at(testIdx);
          if (const double diff = std::abs(current - tmp);
              diff < smallestDifference) {
            smallestDifference = diff;
            bestSymbol = tmp;
          }
        }
        if ((bestSymbol == std::complex<double>(999, 999)) ||
            (smallestDifference == std::numeric_limits<double>::max())) {
          throw std::invalid_argument(
              "Error while finding best corresponding symbol.");
        }
        symbols(i, j) = bestSymbol;
      }
    }
    data = symbols;
  }
};

#endif // DECODER_H