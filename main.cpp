// @author Alexandre P.J. Dixneuf

#include "../decoders/BPDecoder.cpp"
#include "../decoders/GaBPDecoder.cpp"
#include "../decoders/ClusteredDecoder.cpp"
#include "../decoders/LDPCCDecoder.cpp"
#include "../decoders/BICMDecoder.cpp"
#include "Decoder.h"
#include <fstream>
#include <iostream>

// Configure these values for a base test case
constexpr int base_N_t = 2;
constexpr int base_N_r = 2;
constexpr int base_T = 2;
constexpr int base_M = 16;
constexpr int base_SNR = 20;

// Configure these for the minimum values of the simulations
constexpr int N_t_min = 2;
constexpr int N_r_min = 2;
constexpr int T_min = 2;
constexpr int M_min = 16;
constexpr int SNR_min = 10;
// Configure these for the maximum values of the simulations
constexpr int N_t_max = 5;
constexpr int N_r_max = 5;
constexpr int T_max = 10;
constexpr int M_max = 256;
constexpr int SNR_max = 40;


// Basic tolerance, add this to all equality checks using double to avoid
// computer arithmetic errors
constexpr double tol = 1e-6;
// How many trials each simulation should run
constexpr int trial_count = 20;

// TODO - If I had more time, would reverse to binary to do a binary accuracy
// check

// Returns the accuracy (in decimal) of the decoded matrix compared to the true
// data by checking how often the complex terms are equal
double accuracyCheck_ComplexEquality(const Eigen::MatrixXcd &trueData,
                                     const Eigen::MatrixXcd &decodedData) {
  double sum = 0;
  double count = trueData.rows() * trueData.cols();
  // For each symbol in the message
  for (int i = 0; i < trueData.rows(); ++i) {
    for (int j = 0; j < trueData.cols(); ++j) {
      if (std::abs(decodedData(i, j) - trueData(i, j)) < tol) {
        ++sum;
      }
    }
  }
  return sum / count;
}

// Returns the decimal interpretation of the accuracy of the BPDecoder within the given parameters
double decodeUsingBP(const Eigen::MatrixXcd &X, const Eigen::MatrixXcd &Y, const ProblemParameters *p, const QAMConstellation &Q, const Channel &channel) {
  BPDecoder decoder(Y, p, Q, channel);
  const Eigen::MatrixXcd X_hat = decoder.Run();
  return accuracyCheck_ComplexEquality(X, X_hat);
}

// Returns the decimal interpretation of the accuracy of the GaBPDecoder within the given parameters
double decodeUsingGaBP(const Eigen::MatrixXcd &X, const Eigen::MatrixXcd &Y, const ProblemParameters *p, const QAMConstellation &Q, const Channel &channel) {
  GaBPDecoder decoder(Y, p, Q, channel);
  const Eigen::MatrixXcd X_hat = decoder.Run();
  return accuracyCheck_ComplexEquality(X, X_hat);
}

// Returns the decimal interpretation of the accuracy of the ClusteredDecoder within the given parameters
double decodeUsingClustered(const Eigen::MatrixXcd &X, const Eigen::MatrixXcd &Y, const ProblemParameters *p, const QAMConstellation &Q, const Channel &channel) {
  ClusteredDecoder decoder(Y, p, Q, channel);
  const Eigen::MatrixXcd X_hat = decoder.Run();
  return accuracyCheck_ComplexEquality(X, X_hat);
}

// Returns the decimal interpretation of the accuracy of the LDPCCDecoder within the given parameters
double decodeUsingLDPCC(const Eigen::MatrixXcd &X, const Eigen::MatrixXcd &Y, const ProblemParameters *p, const QAMConstellation &Q, const Channel &channel) {
  LDPCCDecoder decoder(Y, p, Q, channel);
  const Eigen::MatrixXcd X_hat = decoder.Run();
  return accuracyCheck_ComplexEquality(X, X_hat);
}

// Returns the decimal interpretation of the accuracy of the BICMDecoder within the given parameters
double decodeUsingBICM(const Eigen::MatrixXcd &X, const Eigen::MatrixXcd &Y, const ProblemParameters *p, const QAMConstellation &Q, const Channel &channel) {
  BICMDecoder decoder(Y, p, Q, channel);
  const Eigen::MatrixXcd X_hat = decoder.Run();
  return accuracyCheck_ComplexEquality(X, X_hat);
}


int main() {
  std::ofstream outFile("../decoder_accuracy.csv");

  // First simulation, increasing N_t
  std::cout << "Simulating Increasing N_t" << std::endl;
  outFile << "Change in Accuracy as N_t Increases\n";
  outFile << "N_t, BP Accuracy, GaBP Accuracy, Clustered Accuracy, LDPCC Accuracy, BICM Accuracy\n";
  for (int N_t = N_t_min; N_t <= N_t_max; N_t++) {
    std::cout << "N_t Simulation Status: " << (N_t*100.0)/N_t_max << "%" << std::endl;
    double BPaccuracy = 0;
    double GaBPaccuracy = 0;
    double Clusteredaccuracy = 0;
    double LDPCCaccuracy = 0;
    double BICMaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      ProblemParameters p(N_t, base_N_r, base_T, base_M, base_SNR);
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      Eigen::MatrixXcd receivedSignal = signal.CopyData();
      BPaccuracy += decodeUsingBP(trueSignal,receivedSignal, &p, Q, channel);
      GaBPaccuracy += decodeUsingGaBP(trueSignal,receivedSignal, &p, Q, channel);
      Clusteredaccuracy += decodeUsingClustered(trueSignal,receivedSignal, &p, Q, channel);
      // LDPCCaccuracy += decodeUsingLDPCC(trueSignal,receivedSignal, &p, Q, channel);
      BICMaccuracy += decodeUsingBICM(trueSignal,receivedSignal, &p, Q, channel);
    }
    outFile << N_t << "," << BPaccuracy / trial_count << "," << GaBPaccuracy / trial_count << "," << Clusteredaccuracy / trial_count << "," << LDPCCaccuracy / trial_count << "," << BICMaccuracy / trial_count << "\n";
  }

  // Second simulation, increasing N_r
  std::cout << "Simulating Increasing N_r" << std::endl;
  outFile << "Change in Accuracy as N_r Increases\n";
  outFile << "N_r, BP Accuracy, GaBP Accuracy, Clustered Accuracy, LDPCC Accuracy, BICM Accuracy\n";
  for (int N_r = N_r_min; N_r <= N_r_max; N_r++) {
    std::cout << "N_r Simulation Status: " << (N_r*100.0)/N_r_max << "%" << std::endl;
    double BPaccuracy = 0;
    double GaBPaccuracy = 0;
    double Clusteredaccuracy = 0;
    double LDPCCaccuracy = 0;
    double BICMaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      ProblemParameters p(base_N_t, N_r, base_T, base_M, base_SNR);
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      Eigen::MatrixXcd receivedSignal = signal.CopyData();
      BPaccuracy += decodeUsingBP(trueSignal,receivedSignal, &p, Q, channel);
      GaBPaccuracy += decodeUsingGaBP(trueSignal,receivedSignal, &p, Q, channel);
      Clusteredaccuracy += decodeUsingClustered(trueSignal,receivedSignal, &p, Q, channel);
      LDPCCaccuracy += decodeUsingLDPCC(trueSignal,receivedSignal, &p, Q, channel);
      BICMaccuracy += decodeUsingBICM(trueSignal,receivedSignal, &p, Q, channel);
    }
    outFile << N_r << "," << BPaccuracy / trial_count << "," << GaBPaccuracy / trial_count << "," << Clusteredaccuracy / trial_count << "," << LDPCCaccuracy / trial_count << "," << BICMaccuracy / trial_count << "\n";
  }

  // Third simulation, increasing T
  std::cout << "Simulating Increasing T" << std::endl;
  outFile << "Change in Accuracy as T Increases\n";
  outFile << "T, BP Accuracy, GaBP Accuracy, Clustered Accuracy, LDPCC Accuracy, BICM Accuracy\n";
  for (int T = T_min; T <= T_max; T = T + 2) {
    std::cout << "T Simulation Status: " << (T*100.0)/T_max << "%" << std::endl;
    double BPaccuracy = 0;
    double GaBPaccuracy = 0;
    double Clusteredaccuracy = 0;
    double LDPCCaccuracy = 0;
    double BICMaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      ProblemParameters p(base_N_t, base_N_r, T, base_M, base_SNR);
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      Eigen::MatrixXcd receivedSignal = signal.CopyData();
      BPaccuracy += decodeUsingBP(trueSignal,receivedSignal, &p, Q, channel);
      GaBPaccuracy += decodeUsingGaBP(trueSignal,receivedSignal, &p, Q, channel);
      Clusteredaccuracy += decodeUsingClustered(trueSignal,receivedSignal, &p, Q, channel);
      LDPCCaccuracy += decodeUsingLDPCC(trueSignal,receivedSignal, &p, Q, channel);
      BICMaccuracy += decodeUsingBICM(trueSignal,receivedSignal, &p, Q, channel);
    }
    outFile << T << "," << BPaccuracy / trial_count << "," << GaBPaccuracy / trial_count << "," << Clusteredaccuracy / trial_count << "," << LDPCCaccuracy / trial_count << "," << BICMaccuracy / trial_count << "\n";
  }

  // Fourth simulation, increasing M
  std::cout << "Simulating Increasing M" << std::endl;
  outFile << "Change in Accuracy as M Increases\n";
  outFile << "M, BP Accuracy, GaBP Accuracy, Clustered Accuracy, LDPCC Accuracy, BICM Accuracy\n";
  for (int M = M_min; M <= M_max; M = M * 4) {
    std::cout << "M Simulation Status: " << (M*100.0)/M_max << "%" << std::endl;
    double BPaccuracy = 0;
    double GaBPaccuracy = 0;
    double Clusteredaccuracy = 0;
    double LDPCCaccuracy = 0;
    double BICMaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      ProblemParameters p(base_N_t, base_N_r, base_T, M, base_SNR);
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      Eigen::MatrixXcd receivedSignal = signal.CopyData();
      BPaccuracy += decodeUsingBP(trueSignal,receivedSignal, &p, Q, channel);
      GaBPaccuracy += decodeUsingGaBP(trueSignal,receivedSignal, &p, Q, channel);
      Clusteredaccuracy += decodeUsingClustered(trueSignal,receivedSignal, &p, Q, channel);
      // LDPCCaccuracy += decodeUsingLDPCC(trueSignal,receivedSignal, &p, Q, channel);
      BICMaccuracy += decodeUsingBICM(trueSignal,receivedSignal, &p, Q, channel);
    }
    outFile << M << "," << BPaccuracy / trial_count << "," << GaBPaccuracy / trial_count << "," << Clusteredaccuracy / trial_count << "," << LDPCCaccuracy / trial_count << "," << BICMaccuracy / trial_count << "\n";
  }

  // Fifth simulation, increasing SNR
  std::cout << "Simulating Increasing SNR" << std::endl;
  outFile << "Change in Accuracy as SNR Increases\n";
  outFile << "SNR, BP Accuracy, GaBP Accuracy, Clustered Accuracy, LDPCC Accuracy, BICM Accuracy\n";
  for (int SNR = SNR_min; SNR <= SNR_max; SNR = SNR + 10) {
    std::cout << "SNR Simulation Status: " << (SNR*100.0)/SNR_max << "%" << std::endl;
    double BPaccuracy = 0;
    double GaBPaccuracy = 0;
    double Clusteredaccuracy = 0;
    double LDPCCaccuracy = 0;
    double BICMaccuracy = 0;
    for (int trial = 0; trial < trial_count; ++trial) {
      ProblemParameters p(base_N_t, base_N_r, base_T, base_M, SNR);
      MySignal signal(&p);
      QAMConstellation Q(&p);
      Channel channel(&p);
      Q.QAMEncoding(signal);
      Eigen::MatrixXcd trueSignal = signal.CopyData();
      channel.ChannelPropagation(signal);
      Eigen::MatrixXcd receivedSignal = signal.CopyData();
      BPaccuracy += decodeUsingBP(trueSignal,receivedSignal, &p, Q, channel);
      GaBPaccuracy += decodeUsingGaBP(trueSignal,receivedSignal, &p, Q, channel);
      Clusteredaccuracy += decodeUsingClustered(trueSignal,receivedSignal, &p, Q, channel);
      LDPCCaccuracy += decodeUsingLDPCC(trueSignal,receivedSignal, &p, Q, channel);
      BICMaccuracy += decodeUsingBICM(trueSignal,receivedSignal, &p, Q, channel);
    }
    outFile << SNR << "," << BPaccuracy / trial_count << "," << GaBPaccuracy / trial_count << "," << Clusteredaccuracy / trial_count << "," << LDPCCaccuracy / trial_count << "," << BICMaccuracy / trial_count << "\n";
  }

  outFile.close();
  std::cout << "Results written to decoder_accuracy.csv\n";
  return 0;
}
