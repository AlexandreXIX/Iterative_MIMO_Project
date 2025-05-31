// @author Alexandre P.J. Dixneuf

#include <map>

#include "Decoder.h"

class BPDecoder : public Decoder {
public:
  virtual Eigen::MatrixXcd decode(const Eigen::MatrixXcd &Y,
                                  const ProblemParameters &myParams,
                                  const QAMConstellation &myQAM,
                                  const Channel &myChannel) override {
    int N_t = myParams.GetNt();
    int N_r = myParams.GetNr();
    int M = myParams.GetM();
    const int num_iters = 5; // Number of BP iterations

    std::unordered_map<int, std::complex<double>> constellation =
        myQAM.GetMapInt2Complex();
    std::vector<int> symbolIndices;
    for (const auto &p : constellation)
      symbolIndices.push_back(p.first);
    MatrixType H = myChannel.GetH();
    double noise_var = 1.0; // Static for now
    double inv_noise = 1.0 / noise_var;

    int T = Y.cols(); // I think this fixed the issue?
    Eigen::MatrixXcd decoded(N_t, T);

    // Loop through time steps (symbols)
    for (int t = 0; t < T; ++t) {
      // Each time step at a time
      Eigen::VectorXcd y_t = Y.col(t);

      // Initialize messages for this time step
      std::vector<std::vector<double>> messages(N_t,
                                                std::vector<double>(M, 0.0));

      // BP iterations
      for (int iter = 0; iter < num_iters; ++iter) {
        std::vector<std::vector<double>> new_messages = messages;

        for (int nt = 0; nt < N_t; ++nt) {
          for (int m_idx = 0; m_idx < M; ++m_idx) {
            std::complex<double> symbol = constellation[symbolIndices[m_idx]];
            double total_likelihood = 0.0;

            for (int nr = 0; nr < N_r; ++nr) {
              std::complex<double> y = y_t(nr);
              std::complex<double> h = H(nr, nt);
              std::complex<double> interference = 0.0;

              for (int other_nt = 0; other_nt < N_t; ++other_nt) {
                if (other_nt == nt)
                  continue;

                int max_idx = std::max_element(messages[other_nt].begin(),
                                               messages[other_nt].end()) -
                              messages[other_nt].begin();
                std::complex<double> s_hat =
                    constellation[symbolIndices[max_idx]];
                interference += H(nr, other_nt) * s_hat;
              }

              std::complex<double> residual = y - (h * symbol + interference);
              double prob = exp(-inv_noise * std::norm(residual));
              total_likelihood += log(prob + 1e-10); // log(0) STUPIDLY BAD
            }

            new_messages[nt][m_idx] = total_likelihood;
          }
        }

        messages = new_messages;
      }

      // Hard decision for this time step
      for (int nt = 0; nt < N_t; ++nt) {
        int max_idx =
            std::max_element(messages[nt].begin(), messages[nt].end()) -
            messages[nt].begin();
        decoded(nt, t) =
            constellation[symbolIndices[max_idx]]; // Storing for now
      }
    }

    return decoded;
  }
};