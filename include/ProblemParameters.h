// @author Alexandre P.J. Dixneuf

#ifndef PROBLEMPARAMETERS_H
#define PROBLEMPARAMETERS_H

#include <cmath>
#include <stdexcept>

class ProblemParameters {
public:
    // Initializer + Constructor
    ProblemParameters(const int &N_t, const int &N_r, const int &T, const int &M);

    // Copy Constructor (deleted)
    ProblemParameters(const ProblemParameters &rhs) = delete;

    // Assignment Operator (deleted)
    ProblemParameters &operator=(const ProblemParameters &rhs) = delete;

    // Destructor (default)
    ~ProblemParameters() = default;

    int GetNt() const;

    int GetNr() const;

    int GetT() const;

    int GetM() const;

    int GetN() const;

private:
    int N_t;
    int N_r;
    int T;
    int M;
    int N;
};

#endif //PROBLEMPARAMETERS_H
