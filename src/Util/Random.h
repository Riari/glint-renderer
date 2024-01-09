#include <random>

namespace Util
{
    // TODO: optimise
    template<typename T>
    T getRandomNumber(T min, T max) {
        std::random_device rd;
        std::mt19937 eng(rd());
        std::uniform_real_distribution<T> dist(min, max);

        return dist(eng);
    }
};
