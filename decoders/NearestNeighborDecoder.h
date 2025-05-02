// @author Alexandre P.J. Dixneuf
#ifndef NEARESTNEIGHBORDECODER_H
#define NEARESTNEIGHBORDECODER_H

#include "Decoder.h"

class NearestNeighborDecoder : public Decoder {
public:
    virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &noisyData) override;
};

#endif // NEARESTNEIGHBORDECODER_H