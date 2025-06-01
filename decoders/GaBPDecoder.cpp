// @author Alexandre P.J. Dixneuf

#include "Decoder.h"
#include <limits>
#include <vector>

class GaBPDecoder : public Decoder {
public:
  GaBPDecoder(const Eigen::MatrixXcd &Y, const ProblemParameters *params,
              const QAMConstellation &myQAM, const Channel &myChannel)
      : Decoder(Y, params, myQAM, myChannel) {}

  void decode() override {
    const int N_t = myParams->GetNt();
    const int N_r = myParams->GetNr();
    const int T = data.cols();
    const int num_iters = 10;
    double noise_var = 1.0;

    Eigen::MatrixXcd decoded(N_t, T);

    // GaBP iterations per time slot
    for (int t = 0; t < T; ++t) {
      Eigen::VectorXcd y = data.col(t);

      // Prior values
      Eigen::VectorXcd miu = Eigen::VectorXcd::Zero(N_t);
      Eigen::VectorXd sigma2 = Eigen::VectorXd::Constant(N_t, 1e5);

      // Run every iteration
      for (int iter = 0; iter < num_iters; ++iter) {
        // For every transmitter
        for (int i = 0; i < N_t; ++i) {
          double sigma2_new = 0.0;
          std::complex<double> miu_new = {0.0, 0.0};

          // Now what did the receiver see
          for (int j = 0; j < N_r; ++j) {
            std::complex<double> hij = H(j, i);
            std::complex<double> residual = y(j);

            // Subtract contributions from all variables except i
            // I hate this step because it makes no sense
            for (int k = 0; k < N_t; ++k) {
              if (k != i) {
                residual -= H(j, k) * miu(k);
              }
            }

            // Why not I guess? At this point, the wikipedia page is my best
            // ally
            double denom = std::norm(hij) / noise_var;
            sigma2_new += denom;
            miu_new += std::conj(hij) * residual / noise_var;
          }

          if (sigma2_new > 0) {
            sigma2(i) = 1.0 / sigma2_new;
            miu(i) = sigma2(i) * miu_new;
          }
        }
      }

      decoded.col(t) = miu;
    }
    data = decoded;
  }
};

// TODO - add a log function to handle large values? Needed because overflow in
// BP, but doesn't seem to be an issue here?