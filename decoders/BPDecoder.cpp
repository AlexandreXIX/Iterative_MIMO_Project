// @author Alexandre P.J. Dixneuf

#include <map>

#include "Decoder.h"

class BPDecoder : public Decoder {
public:
  virtual Eigen::MatrixXcd decode() override {

    int N_t = params->GetNt();
    int N_r = params->GetNr();
    int M = params->GetM();
    int T = data.cols(); // number of timesteps or messages sent
    const int num_iters = 5; // Number of BP iterations
    Eigen::MatrixXcd decoded(N_t, T);

    double noise_var = 1.0; // Static for now
    double inv_noise = 1.0 / noise_var;

    // Handle each time step independently
    for (int t = 0; t < T; ++t) {
      Eigen::VectorXcd y = data.col(t); // This should result in (N_r x 1)
      // Initialize a message
      // Each symbol x_j to observation y_i has an equal initial chance of occurring
      // TODO - do I initialize to 0 or 1/M
      std::vector<std::vector<double>> messages(N_t, std::vector<double>(N_r, 0.0));
      // Iterate
      for (int iter = 0; iter < num_iters; ++iter) {
        // Update observation y_i to symbol x_j
        for (int i = 0; i < N_t; ++i) {
          for (int j = 0; j < N_r; ++j) {
            // Compute message
            // TODO - figure out this line
          }
        }
        // Update message from symbol x_j to observation y_i
        for (int j = 0; j < N_r; ++j) {
          for (int i = 0; i < N_t; ++i) {
            // Combine incoming
            // Compute
            // TODO - figure out compute

          }
        }
      }
      // Figure out final message at this time step
    }
  }
};
