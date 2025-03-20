// @author Alexandre P.J. Dixneuf

#ifndef QAMCONSTELLATION_H
#define QAMCONSTELLATION_H

#include "Signal.h"

/**
 * @class QAMConstellation
 * @brief This class serves to handle the tasks involving the QAM Constellation.
 * @details There are three main tasks this class should handle:
 *          1) Generate a grid representing a gray-code M-QAM constellation.
 *          2) Takes a Signal object with integer values and encodes them into QAM using the generated constellation.
 *          3) Takes an encoded Signal object and decodes them using the constellation back into integer values.
 *          It is important to note that this class only stores the constellation. Re-encoding and decoding using the
 *          same constellation is possible by keeping the same instance of this class.
 *          Note that the container also stores N = sqrt(M) to avoid recalculation.
 * @param This class needs 1 parameter: M
 *        M: Since the constellation must be standalone to the signal to be reusable, needs to store M. This will also
 *        serve to ensure no bugs from a signal expecting a higher level constellation. Will throw an error if the
 *        opposite is true, but will still output a result.
 */

class QAMConstellation {
public:
    // Initializer + Constructor
    QAMConstellation(const int M);

    // Copy Constructor (banned)
    QAMConstellation(const QAMConstellation &rhs) = delete;

    // Assignment Operator (banned)
    QAMConstellation &operator=(const QAMConstellation &rhs) = delete;

    // Deconstructor (default)
    ~QAMConstellation() = default;

    // Converts Signal int to complex
    void QAMEncoding(Signal &mySignal);

    // Converts Signal complex to int
    void QAMDecoding(Signal &mySignal);

    // Draws the constellation to assist testing and debugging
    void DrawConstellation();

private:
    int M;
    int N;
    std::vector<std::vector<int>> Constellation;
};



#endif //QAMCONSTELLATION_H
