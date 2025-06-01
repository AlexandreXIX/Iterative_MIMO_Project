// @author Alexandre P.J. Dixneuf

#include "Decoder.h"
#include <algorithm>
#include <iostream>
#include <numeric>
#include <random>
#include <vector>

// TODO - recheck code

class ClusteredDecoder : public Decoder {
public:
  ClusteredDecoder(const Eigen::MatrixXcd &Y, const ProblemParameters *params,
                   const QAMConstellation &myQAM, const Channel &myChannel)
      : Decoder(Y, params, myQAM, myChannel) {}

  void decode() override {
    const int N_t = myParams->GetNt();
    const int N_r = myParams->GetNr();
    const int M = myParams->GetM();
    const int T = data.cols();
    const int num_iters = 10;
    const int cluster_size = std::min(4, N_t); // Subgroup size
    double noise_var = 1.0;

    // Again regretting how I made the QAM, MATLAB has it basically built-in
    std::vector<std::complex<double>> constellation_points(M);
    for (int i = 0; i < M; ++i)
      constellation_points[i] = constellation[i];

    Eigen::MatrixXcd decoded(N_t, T);

    for (int t = 0; t < T; ++t) {
      Eigen::VectorXcd y = data.col(t);

      // Initial guesses
      Eigen::VectorXcd miu = Eigen::VectorXcd::Zero(N_t);
      Eigen::VectorXd var = Eigen::VectorXd::Constant(N_t, 1e5);

      // Run each iteration
      for (int iter = 0; iter < num_iters; ++iter) {

        // Go through each cluster
        // TODO - change this, it works but looks ugly
        for (int start = 0; start < N_t; start += cluster_size) {
          int end = std::min(start + cluster_size, N_t);
          int K = end - start;

          // Generate sub blocks (in other words, basically clusters)
          Eigen::MatrixXcd H_sub = H.block(0, start, N_r, K);
          Eigen::VectorXcd miu_sub = miu.segment(start, K);
          Eigen::VectorXd var_sub = var.segment(start, K);

          // Everything else is treated as noise
          Eigen::VectorXcd y_eff = y;
          for (int j = 0; j < N_t; ++j) {
            if (j < start || j >= end)
              y_eff -= H.col(j) * miu(j);
          }

          Eigen::MatrixXcd R = H_sub.adjoint() * H_sub +
                               noise_var * Eigen::MatrixXcd::Identity(K, K);
          Eigen::VectorXcd rhs = H_sub.adjoint() * y_eff;

          Eigen::VectorXcd mu_update = R.ldlt().solve(rhs);
          miu.segment(start, K) = mu_update;

          // (Optionally) Update variances
          for (int k = 0; k < K; ++k) {
            var(start + k) = 1.0 / R(k, k).real();
          }
        }
      }

      // Map final means to the closest constellation point
      // This is where we determine the final guess
      for (int i = 0; i < N_t; ++i) {
        std::complex<double> bestSym(999, 999);
        double min_dist = std::numeric_limits<double>::max();
        for (const auto &s : constellation_points) {
          double dist = std::norm(miu(i) - s);
          if (dist < min_dist) {
            min_dist = dist;
            bestSym = s;
          }
        }
        decoded(i, t) = bestSym;
      }
    }

    data = decoded;
  }
};
