// @author Alexandre P.J. Dixneuf

#include "Decoder.h"

class BPDecoder : public Decoder {
public:
  virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &Y, const ProblemParameters &myParams, const QAMConstellation &myQAM, const Channel &myChannel) override {
    // Get problem constants
    int N_t = myParams.GetNt();
    int N_r = myParams.GetNr();
    int M = myParams.GetM();
    // Get constellation
    std::unordered_map<int, std::complex<double>> constellation = myQAM.GetMapInt2Complex();
    // Get channel conditions CSIR
    MatrixType H = myChannel.GetH();
    int noise_var = 1; // This is fixed for this simulation
    // Start decoding
    std::vector<std::vector<std::vector<double>>> messages;
    // messages[nt][nr][sym_idx] = message from variable node nt to factor node nr about symbol sym_idx

    for (int nt = 0; nt < N_t; nt++) {
      for (int nr = 0; nr < N_r; nr++) {
        for (const auto& pair : constellation) {
          std::complex<double> symbol = pair.second;
          std::complex<double> estimate = H(nt,nr) * symbol;
          double prob = exp(-norm(Y(nt) - estimate) / noise_var);
          messages[nt][nr][pair.first] = prob;
          // TODO - add missing work here, fix up the loops
        }
      }
    }
    // TODO - added decoder
    return decoded;
  }
};
