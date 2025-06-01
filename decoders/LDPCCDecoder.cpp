// @author Alexandre P.J. Dixneuf

#include "Decoder.h"
#include <bitset>
#include <random>
#include <cmath>

class LDPCCDecoder : public Decoder {
public:
  LDPCCDecoder(const Eigen::MatrixXcd &Y, const ProblemParameters *params,
               const QAMConstellation &myQAM, const Channel &myChannel)
      : Decoder(Y, params, myQAM, myChannel) {
    buildLDPCMatrix();
    buildConstellationBitMap();
  }

  void decode() override {
    const int N_t = myParams->GetNt();
    const int T = data.cols();
    const int M = myParams->GetM();
    const int m = H_ldpc.rows();
    const int n = H_ldpc.cols();
    // Do I use this? I saved this in the other files, so who knows
    const double noise_var = 1.0;

    Eigen::MatrixXcd decoded(N_t, T);

    // Run for every time point
    for (int t = 0; t < T; ++t) {
      Eigen::VectorXcd y = data.col(t);

      // Initiate the data
      std::vector<double> llr(n, 0.0);
      for (int i = 0; i < N_t; ++i) {
        for (int m_idx = 0; m_idx < M; ++m_idx) {
          std::complex<double> sym = constellation.at(m_idx);
          Eigen::VectorXcd x = Eigen::VectorXcd::Zero(H.cols());
          x(i) = sym;
          std::complex<double> estimate = H.row(i) * x;
          double dist = std::norm(y(i) - estimate);
          int bits = symbolBits[m_idx];
          for (int b = 0; b < N; ++b) {
            llr[i * N + b] += (bits & (1 << b)) ? -dist : dist;
          }
        }
      }

      // Messages: variable-to-check and check-to-variable
      // Hate this notation, but everything I see uses this, so when in Rome
      std::vector<std::vector<double>> msg_v_to_c(n);
      std::vector<std::vector<double>> msg_c_to_v(n);
      for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
          if (H_ldpc(j, i) == 1) {
            msg_v_to_c[i].push_back(llr[i]);
            msg_c_to_v[i].push_back(0.0);
          }
        }
      }

      // Now run for every iteration
      for (int iter = 0; iter < num_iters; ++iter) {
        // Check vars
        for (int j = 0; j < m; ++j) {
          std::vector<int> indices;
          // Check every index
          for (int i = 0; i < n; ++i)
            if (H_ldpc(j, i) == 1) indices.push_back(i);

          // For every index
          for (size_t idx = 0; idx < indices.size(); ++idx) {
            double prod = 1.0;
            // Run
            for (size_t k = 0; k < indices.size(); ++k) {
              if (k != idx) {
                int i_k = indices[k];
                double tanh_val = std::tanh(msg_v_to_c[i_k][j] / 2.0);
                prod *= tanh_val;
              }
            }
            int i = indices[idx];
            msg_c_to_v[i][j] = 2.0 * atanh(prod);
          }
        }

        // Variable to check
        for (int i = 0; i < n; ++i) {
          for (size_t j_idx = 0; j_idx < msg_v_to_c[i].size(); ++j_idx) {
            double sum = llr[i];
            for (size_t k = 0; k < msg_c_to_v[i].size(); ++k) {
              if (k != j_idx) sum += msg_c_to_v[i][k];
            }
            msg_v_to_c[i][j_idx] = sum;
          }
        }
      }

      // Decide
      std::vector<int> bits(n);
      for (int i = 0; i < n; ++i) {
        double sum = llr[i];
        for (auto &v : msg_c_to_v[i])
          sum += v;
        bits[i] = sum < 0 ? 1 : 0;
      }

      // Map bits back to symbols
      // Technically in Decoder, but this is easier? I think? I want to go to sleep
      for (int i = 0; i < N_t; ++i) {
        int idx = 0;
        for (int b = 0; b < N; ++b) {
          idx |= (bits[i * N + b] << b);
        }
        decoded(i, t) = constellation.at(idx);
      }
    }

    data = decoded;
  }

private:
  // This is the H matrix that is random unless following a real-world algorithm
  Eigen::MatrixXi H_ldpc;
  // This should be in QAMConstellation, but too late
  std::unordered_map<int, int> symbolBits; // symbol index -> bit representation
  // This is just N
  int N;

  void buildConstellationBitMap() {
    N = myParams->GetN();
    for (const auto &[symbol_int, value] : constellation) {
      symbolBits[symbol_int] = grayToBinary(symbol_int);
    }
  }

  static int grayToBinary(int gray) {
    int binary = 0;
    for (; gray; gray >>= 1)
      binary ^= gray;
    return binary;
  }

  void buildLDPCMatrix() {
    // Simple regular (3, 6) LDPC matrix example
    // Size based on bits per symbol and transmitters
    // Should maybe hard-code this later
    // TODO - do more work on this matrix
    const int n = myParams->GetNt() * std::log2(myParams->GetM());
    const int dv = 3;
    const int dc = 6;
    const int m = n * dv / dc;

    H_ldpc = Eigen::MatrixXi::Zero(m, n);
    // RNG 19
    std::mt19937 rng(19);
    std::uniform_int_distribution<> dist(0, m - 1);

    for (int i = 0; i < n; ++i) {
      int filled = 0;
      while (filled < dv) {
        int row = dist(rng);
        // I think this leads to infinite loops
        // TODO - debug
        if (H_ldpc(row, i) == 0) {
          H_ldpc(row, i) = 1;
          filled++;
        }
      }
    }
  }

  // Should have done this in MATLAB
  static double atanh(double x) {
    return 0.5 * std::log((1 + x) / (1 - x));
  }
};
