// @author Alexandre P.J. Dixneuf

#include "QAMConstellation.h"

#include <System.h>

// Initializer + Constructor
QAMConstellation::QAMConstellation(const int M) : M(M), N(static_cast<int>(sqrt(M))), Constellation(N, std::vector<int>(N)) {
    // Currently, works only on perfect squares that are powers of 2 (4, 16, 64, 256, etc.)
    if (N * N != M) {
        throw std::invalid_argument("M must be a perfect square.");
    }
    int power2Test = static_cast<int>(std::log2(M));
    if (pow(2, power2Test) != M) {
        throw std::invalid_argument("M is not a power of 2.");
    }
    // M = 2 wierd edge case and never useful, better to throw out (<= also removes negatives just in case)
    if (M <= 2) {
        throw std::invalid_argument("M is too small.");
    }
    // Now, construct the constellation
    // Generate initial
    std::vector<Num> tmpVector(M);
    for (int i = 0; i < M; i++) {
        tmpVector[i] = (i ^ (i >> 1)); // Generating Gray code
    }
    // Now we convert to a properly ordered grid to simulate graphing
    int index = 0;
    for (int i = 0; i < N; i++) {
        // Left to right
        if (i % 2 == 0) {
            for (int j = 0; j < N; j++) {
                Constellation[i][j] = tmpVector[index++];
            }
        }
        // Right to left (snake pattern)
        else {
            for (int j = N - 1; j >= 0; j--) {
                Constellation[i][j] = tmpVector[index++];
            }
        }
    }
}

// Converts Signal int to complex
void QAMConstellation::QAMEncoding(Signal &mySignal) {
    // TODO - complete
}

// Converts Signal complex to int
void QAMConstellation::QAMDecoding(Signal &mySignal) {
    // TODO - complete
}

void QAMConstellation::DrawConstellation() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << Constellation[i][j] << " ";
        }
        std::cout << std::endl;
    }
}