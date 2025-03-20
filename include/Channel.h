// @author Alexandre P.J. Dixneuf

#ifndef CHANNEL_H
#define CHANNEL_H

#include "MySignal.h"

/**
 * @class Channel
 * @brief This class handles the channel conditions.
 * @details There are three main tasks this class should handle:
 *          1) Generate random noise and Rayleigh fading (H and Z)
 *          2) Take a signal and distort it with channel conditions
 * @param ProblemParameters object
 */

class Channel {
public:
    // Initializer + Constructor
    Channel(const ProblemParameters* params);

    // Copy Constructor (banned)
    Channel(const Channel &rhs) = delete;

    // Assignment Operator (banned)
    Channel &operator=(const Channel &rhs) = delete;

    // Deconstructor (delete each user)
    ~Channel() = default;

    // Take in a signal, and modulate it according to the channel conditions
    void ChannelModulation(MySignal &transmitted) const;

    // Outputs the problem parameters pointer to verify
    const ProblemParameters* GetParameters() const;

    // Checks if a given pointer is the same pointer as this object's parameters (output True means same)
    bool SameParameters(const ProblemParameters* otherPointer) const;

private:
    MatrixType H;
    MatrixType Z;
    const ProblemParameters* params;
};

#endif //CHANNEL_H
