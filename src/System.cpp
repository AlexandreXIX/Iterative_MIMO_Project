// @author Alexandre P.J. Dixneuf

#include "../include/System.h"
#include <iostream>

typedef int Num;

System::System(const Num &userCount, const Num &receiverCount, const Num &time,
               const Num &dim)
    : userCount(userCount), receiverCount(receiverCount), time(time), dim(dim) {
    Users.reserve(userCount);
    for (Num i = 0; i < userCount; i++) {
        Users.emplace_back(time,dim);
    }
}
