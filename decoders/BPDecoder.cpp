// @author Alexandre P.J. Dixneuf

#include <map>

#include "Decoder.h"

class BPDecoder : public Decoder {
public:
  BPDecoder(const Eigen::MatrixXcd &Y, const ProblemParameters *params,
            const QAMConstellation &myQAM, const Channel &myChannel)
      : Decoder(Y, params, myQAM, myChannel) {}

  void decode() override {
    const int N_t = myParams->GetNt();
    const int N_r = myParams->GetNr();
    const int M = myParams->GetM();
    const int T = data.cols();
    const int num_iters = 10;
    double noise_var = 1.0;
    double inv_noise = 1.0 / noise_var;

    // Constellation converts to vector
    // Honestly, starting with a map was not the best move
    std::vector<std::complex<double>> constellation_points(M);
    for (int i = 0; i < M; ++i)
      constellation_points[i] = constellation[i];

    Eigen::MatrixXcd decoded(N_t, T); // Shape of X (what we want), not Y

    for (int t = 0; t < T; ++t) {
      Eigen::VectorXcd y = data.col(t);

      // Added this to avoid issues with large numbers, logs necessart
      std::vector<std::vector<std::vector<double>>> log_msg_var_to_fact(
        N_t, std::vector<std::vector<double>>(N_r, std::vector<double>(M, std::log(1.0 / M))));
      std::vector<std::vector<std::vector<double>>> log_msg_fact_to_var(
        N_r, std::vector<std::vector<double>>(N_t, std::vector<double>(M, 0.0)));

      // Go over every iteration
      for (int iter = 0; iter < num_iters; ++iter) {
        // Treat each message received independently
        for (int nr = 0; nr < N_r; ++nr) {
          // For each component transmitted
          for (int nt = 0; nt < N_t; ++nt) {
            // For every symbol
            for (int m = 0; m < M; ++m) {
              std::vector<double> log_terms;
              const int combos = std::pow(M, N_t - 1);
              for (int c = 0; c < combos; ++c) {
                std::vector<int> symbol_idx(N_t);
                int temp = c;
                for (int k = 0, j = 0; k < N_t; ++k) {
                  if (k == nt) {
                    symbol_idx[k] = m;
                  } else {
                    symbol_idx[k] = temp % M;
                    temp /= M;
                  }
                }

                std::complex<double> sum = {0.0,0.0};
                double log_prior = 0.0;
                for (int k = 0; k < N_t; ++k) {
                  sum += H(nr, k) * constellation_points[symbol_idx[k]];
                  if (k != nt)
                    log_prior += log_msg_var_to_fact[k][nr][symbol_idx[k]];
                }

                std::complex<double> diff = std::abs(y(nr) - sum);
                double log_likelihood = -std::norm(diff) * inv_noise;
                log_terms.push_back(log_likelihood + log_prior);
              }

              log_msg_fact_to_var[nr][nt][m] = logSumExp(log_terms);
            }

            // Normalize log-probs
            double log_norm = logSumExp(log_msg_fact_to_var[nr][nt]);
            for (int m = 0; m < M; ++m)
              log_msg_fact_to_var[nr][nt][m] -= log_norm;
          }
        }

        // Variable to factor (log)
        for (int t_var = 0; t_var < N_t; ++t_var) {
          for (int r = 0; r < N_r; ++r) {
            for (int m = 0; m < M; ++m) {
              double log_sum = 0.0;
              for (int rp = 0; rp < N_r; ++rp) {
                if (rp != r)
                  log_sum += log_msg_fact_to_var[rp][t_var][m];
              }
              log_msg_var_to_fact[t_var][r][m] = log_sum;
            }

            // Normalize
            double log_norm = logSumExp(log_msg_var_to_fact[t_var][r]);
            for (int m = 0; m < M; ++m)
              log_msg_var_to_fact[t_var][r][m] -= log_norm;
          }
        }
      }

      // Final belief for each variable node (Tx symbol)
      for (int t_var = 0; t_var < N_t; ++t_var) {
        std::vector<double> final_log_belief(M, 0.0);
        for (int m = 0; m < M; ++m) {
          for (int r = 0; r < N_r; ++r)
            final_log_belief[m] += log_msg_fact_to_var[r][t_var][m];
        }

        int best_idx = std::distance(
          final_log_belief.begin(),
          std::max_element(final_log_belief.begin(), final_log_belief.end()));
        decoded(t_var, t) = constellation_points[best_idx];
      }
    }
    data = decoded;
  }

  static double logSumExp(const std::vector<double>& log_probs) {
    // TODO - remove once testing is done
    std::cout << log_probs.size() << std::endl;
    double max_val = *std::max_element(log_probs.begin(), log_probs.end());
    double sum = 0.0;
    for (const auto& lp : log_probs) {
      sum += std::exp(lp - max_val);
    }
    return max_val + std::log(sum);
  }
};