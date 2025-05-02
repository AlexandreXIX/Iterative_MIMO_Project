// @author Alexandre P.J. Dixneuf

#include "Decoder.h"

class NearestNeighborDecoder : public Decoder {
public:
  virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &noisyData, const ProblemParameters myParams, const QAMConstellation myQAM) override {
    // Get the constellation from myQAM
    std::vector<std::vector<int>> constellation = myQAM.GetConstellation();




    return noisyData; // TODO - added decoder
  }
};
