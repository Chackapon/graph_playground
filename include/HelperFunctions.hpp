//
// Created by Mateusz M on 08/05/2026.
//

#ifndef ALGOSY2GRAFY_HELPERFUNCTIONS_H
#define ALGOSY2GRAFY_HELPERFUNCTIONS_H
#include <ostream>
#include <random>
#include <sstream>
#include <string>

// TODO separate into different helper headers (maybe with cpp files)

inline std::string yn(const bool expression) {
    return expression ? "YES" : "NO";
}


inline std::string str( std::string arg ) {
    return arg;
}

inline std::string str(const void* arg) {
    std::ostringstream result;
    result << arg;
    return result.str();
}

template <typename T>
auto str( T arg ) -> std::enable_if_t< std::is_arithmetic_v<T>, std::string >  {
    return std::to_string( arg );
}

template <typename Container>
auto str(const Container& cnt) -> decltype( cnt.begin(), cnt.end(), std::string() ) {
    std::ostringstream result; result << "[";
    bool first = true;

    for (const auto &e : cnt ) {
        if (!first) result << ", ";
        result << e;
        first = false;
    }

    result << "]";
    return result.str();
}


inline void rand_init() {
    std::random_device rd;
    srand( rd() ); //TODO update to new C++ random standard
}

inline float rand_float() {
    const float r = (float)(rand()) / (float)(RAND_MAX);
    return r == 0 ? 1 : r;
}

inline int rand_int(int a , int b = 0) {
    if (b==0) {
        return rand() % a;
    }else{
        return a + rand() % ( b );
    }
}

inline float round_float(float x, int precision = 1) {
    const auto power_of_10 = (float)std::pow(10, precision);
    return std::round(x * power_of_10)  / power_of_10;
}






#endif //ALGOSY2GRAFY_HELPERFUNCTIONS_H