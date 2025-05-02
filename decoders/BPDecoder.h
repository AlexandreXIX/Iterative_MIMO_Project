// @author Alexandre P.J. Dixneuf
#ifndef BPDECODER_H
#define BPDECODER_H

#include "Decoder.h"

class BPDecoder : public Decoder {
public:
  virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &noisyData) override;
};

#endif // BPDECODER_H