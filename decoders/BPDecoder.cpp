// @author Alexandre P.J. Dixneuf

#include "Decoder.h"

class BPDecoder final : public Decoder {
public:
  Eigen::MatrixXcd decode(const Eigen::MatrixXcd &Y, const ProblemParameters &myParams, const QAMConstellation &myQAM, const Channel &myChannel) override {
    int N_t = myParams.GetNt();
    int N_r = myParams.GetNr();
    int M = myParams.GetM(); // Constellation size (e.g., 16 for 16-QAM)
    const int num_iters = 5; // Number of BP iterations

    // Get QAM mapping and channel
    std::unordered_map<int, std::complex<double>> constellation = myQAM.GetMapInt2Complex();
    std::vector<int> symbolIndices;
    for (const auto& p : constellation) symbolIndices.push_back(p.first);
    MatrixType H = myChannel.GetH();

    // Precompute inverse noise variance
    double noise_var = 1.0; // Assumed known and fixed
    double inv_noise = 1.0 / noise_var;

    // Initialize messages: messages[nt][sym_idx] = log-belief for symbol sym_idx
    std::vector<std::vector<double>> messages(N_t, std::vector<double>(M, 0.0));

    // Initialize: uniform priors
    for (int nt = 0; nt < N_t; ++nt) {
      for (int m = 0; m < M; ++m) {
        messages[nt][m] = 0.0; // log(1/M) is same for all, can be ignored
      }
    }

    // BP iterations
    for (int iter = 0; iter < num_iters; ++iter) {
      std::vector<std::vector<double>> new_messages = messages; // Placeholder for updates

      for (int nt = 0; nt < N_t; ++nt) {
        for (int m_idx = 0; m_idx < M; ++m_idx) {
          std::complex<double> symbol = constellation[symbolIndices[m_idx]];
          double total_likelihood = 0.0;

          for (int nr = 0; nr < N_r; ++nr) {
            std::complex<double> y = Y(nr, 0); // Assume 1D signal vector
            std::complex<double> h = H(nr, nt);
            std::complex<double> interference = 0.0;

            // Estimate interference from all other transmit antennas
            for (int other_nt = 0; other_nt < N_t; ++other_nt) {
              if (other_nt == nt) continue;
              // Use most likely symbol for other_nt (MAP estimate)
              int max_idx = std::max_element(messages[other_nt].begin(), messages[other_nt].end()) - messages[other_nt].begin();
              std::complex<double> s_hat = constellation[symbolIndices[max_idx]];
              interference += H(nr, other_nt) * s_hat;
            }

            std::complex<double> residual = y - (h * symbol + interference);
            double prob = exp(-inv_noise * std::norm(residual));
            total_likelihood += log(prob + 1e-10); // add small epsilon to avoid log(0)
          }

          new_messages[nt][m_idx] = total_likelihood;
        }
      }

      messages = new_messages; // Update
    }

    // Make hard decisions
    Eigen::MatrixXcd decoded(N_t, 1);
    for (int nt = 0; nt < N_t; ++nt) {
      int max_idx = std::max_element(messages[nt].begin(), messages[nt].end()) - messages[nt].begin();
      decoded(nt, 0) = constellation[symbolIndices[max_idx]];
    }

    return decoded;
  }
};
