// @author Alexandre P.J. Dixneuf

#ifndef DECODER_H
#define DECODER_H

#include <Eigen/Dense>
#include "QAMConstellation.h"
#include "Channel.h"


/**
 * @class Decoder
 * @brief A virtual function to serve as a template for future decoders
 * @details This sets the requirements for a decoder function to work with the rest of the code. It must be capable of handling a ProblemParameters object, to get the necessary problem daa. In addition; it must take in and output Eigen::MatrixXcd as the input and output data. Lastly, it handles the QAMConstellation class to be able to convert int to complex and complex to int
 * @param ProblemParameters object, QAMConstellation object
 */

class Decoder {
public:
  // TODO - create a unified constructor so that any future coder can assume the variables already exist
  // Decoder(const ProblemParameters *params);
  virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &Y, const ProblemParameters &myParams, const QAMConstellation &myQAM, const Channel &myChannel) = 0; // Pure virtual function
  virtual ~Decoder() = default;
};

#endif // DECODER_H