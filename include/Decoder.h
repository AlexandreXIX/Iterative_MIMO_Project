// @author Alexandre P.J. Dixneuf

#ifndef DECODER_H
#define DECODER_H

#include <Eigen/Dense>

class Decoder {
public:
  virtual Eigen::MatrixXcd
  decode(const Eigen::MatrixXcd &noisyData) = 0; // Pure virtual function
  virtual ~Decoder() = default;
};

#endif // DECODER_H