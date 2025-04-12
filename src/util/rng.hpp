#pragma once

#include <random>
namespace tiny_cherno {

    template <typename T> T random() {
        static std::mt19937 gen{std::random_device{}()}; 
        static std::uniform_int_distribution<T> dist(
            std::numeric_limits<T>::min(),
            std::numeric_limits<T>::max()
        );
        return dist(gen);
    }

}
