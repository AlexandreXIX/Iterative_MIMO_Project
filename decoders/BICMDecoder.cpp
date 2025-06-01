#include "Decoder.h"
#include <bitset>
#include <cmath>

class BICMDecoder : public Decoder {
public:
  BICMDecoder(const Eigen::MatrixXcd &Y, const ProblemParameters *params,
              const QAMConstellation &myQAM, const Channel &myChannel)
      : Decoder(Y, params, myQAM, myChannel),
        constellationVec(params->GetM()) {
    for (int i = 0; i < params->GetM(); ++i) {
      constellationVec[i] = constellation[i];
    }

    N = myParams->GetN();
  }

  void decode() override {
    const int N_t = myParams->GetNt();
    const int N_r = myParams->GetNr();
    const int T = data.cols();
    double noise_var = 1.0;
    // Useful here
    double inv_noise = 1.0 / noise_var;

    Eigen::MatrixXcd decoded(N_t, T);

    // For every transmitted message
    for (int t = 0; t < T; ++t) {
      Eigen::VectorXcd y = data.col(t);

      // Check every transmitter
      for (int nt = 0; nt < N_t; ++nt) {
        std::vector<double> LLR(N, 0.0);

        // For each bit position
        for (int bitPos = 0; bitPos < N; ++bitPos) {
          std::vector<double> ll0, ll1;

          // For every constellation value
          for (int m = 0; m < constellationVec.size(); ++m) {
            std::bitset<8> bits(m);
            int bit = bits[bitPos];

            // Build transmit vector with only tx nt using x_m
            Eigen::VectorXcd x = Eigen::VectorXcd::Zero(myParams->GetNt());
            x(nt) = constellationVec[m];

            // This is probably wrong
            // TODO - debug
            Eigen::VectorXcd hx = H * x;
            Eigen::VectorXcd err = y - hx;
            // This should be likelihood
            double metric = -err.squaredNorm() * inv_noise;


            if (bit == 0)
              ll0.push_back(metric);
            else
              ll1.push_back(metric);
          }

          LLR[bitPos] = logSumExp(ll0) - logSumExp(ll1);
        }

        // Hard decision: choose symbol whose bits best match LLR signs
        // AGAIN: should have used the Decoder function, but this is getting tough
        int best_m = 0;
        double best_score = -1e9;
        for (int m = 0; m < constellationVec.size(); ++m) {
          std::bitset<8> bits(m);
          double score = 0.0;
          for (int bitPos = 0; bitPos < N; ++bitPos) {
            int b = bits[bitPos];
            score += (1 - 2 * b) * LLR[bitPos];  // if LLR > 0 → prefer 0
          }

          if (score > best_score) {
            best_score = score;
            best_m = m;
          }
        }

        decoded(nt, t) = constellationVec[best_m];
      }
    }

    data = decoded;
  }

private:
  std::vector<std::complex<double>> constellationVec;
  int N;

  static double logSumExp(const std::vector<double> &v) {
    if (v.empty()) return -std::numeric_limits<double>::infinity();
    double maxVal = *std::max_element(v.begin(), v.end());
    double sum = 0.0;
    for (double x : v)
      sum += std::exp(x - maxVal);
    return maxVal + std::log(sum);
  }
};

