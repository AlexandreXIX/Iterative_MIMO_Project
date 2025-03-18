// @author Alexandre P.J. Dixneuf
#ifndef BPDECODER_H
#define BPDECODER_H

#include "Decoder.h"

class SimpleDecoder : public Decoder {
public:
  virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &noisyData) override {
    return noisyData * 100; // TODO - remove placeholder line
  }
};

#endif // BPDECODER_H