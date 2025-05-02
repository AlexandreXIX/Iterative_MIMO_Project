// @author Alexandre P.J. Dixneuf

#include "Decoder.h"

class BPDecoder : public Decoder {
public:
  virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &noisyData, const ProblemParameters myParams, const QAMConstellation myQAM) override {
    return noisyData; // TODO - added decoder
  }
};
