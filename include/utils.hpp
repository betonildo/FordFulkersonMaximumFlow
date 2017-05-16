#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <signal.h>
#include <chrono>

#define MAX(a, b) ((a) < (b) ? (b) : (a))

#define MIN(a, b) ((a) > (b) ? (b) : (a))

#define PHI ((1.0 + sqrt(5.0)) / 2.0)
#define logBase(x, b) (log((double)(x)) / log((double)(b)))
#define Make2Pot(x) {\
        double ___AXHOADISJDOJQWDQ_L = logBase((*x), 2); \
        double ___AXHAOSDJSODIJOQW_intpart; \
        double ___AXHDASIUHDIUWQHD_fractpart = modf (___AXHOADISJDOJQWDQ_L, &___AXHAOSDJSODIJOQW_intpart); \
        (*x) = ___AXHDASIUHDIUWQHD_fractpart > 0 ? pow(2, ceil(___AXHOADISJDOJQWDQ_L)) : (*x); \
}

#define MeasureMS(outtime, codeblock) {\
    auto _DWDAJSDUIUQWIUDQIWUDH___start = std::chrono::system_clock::now(); \
    {codeblock;} \
    auto _QUIDMQIDMIQWUDMEMVIQV_____end = std::chrono::system_clock::now(); \
    (*(outtime)) = std::chrono::duration_cast<std::chrono::milliseconds>(_QUIDMQIDMIQWUDMEMVIQV_____end - _DWDAJSDUIUQWIUDQIWUDH___start).count(); \
}

#endif /*UTILS_H*/