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
      : data(Y), params(params), MapInt2Complex(myQAM.GetMapInt2Complex()),
        H(myChannel.GetH()), Z(myChannel.GetZ()) {}
  void Run() {
    decode();
    complexToSymbol();
  }
  virtual ~Decoder() = default;

private:
  virtual Eigen::MatrixXcd decode() = 0; // Pure virtual function
  void complexToSymbol() {
    Eigen::MatrixXcd symbols = data;
    for (int i = 0; i < data.rows(); i++) {
      for (int j = 0; j < data.cols(); j++) {
        std::complex<double> current = data(i, j);
        std::complex<double> bestSymbol(999, 999);
        double smallestDifference = 999;
        for (int testIdx = 0; testIdx < params->GetM(); testIdx++) {
          std::complex<double> tmp = MapInt2Complex.at(0);
          const double diff = std::sqrt(pow(current.real() - tmp.real(), 2) +
                                        pow(current.imag() - tmp.imag(), 2));
          if (diff < smallestDifference) {
            smallestDifference = diff;
            bestSymbol = tmp;
          }
        }
        if ((bestSymbol == std::complex<double>(999, 999)) ||
            (smallestDifference == 999)) {
          throw std::invalid_argument(
              "Error while finding best corresponding symbol.");
        }
        symbols(i, j) = bestSymbol;
      }
    }
  }

  Eigen::MatrixXcd data;
  const ProblemParameters *params;
  std::unordered_map<int, std::complex<double>> MapInt2Complex;
  Eigen::MatrixXcd H;
  Eigen::MatrixXcd Z;
};

#endif // DECODER_H