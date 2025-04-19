#pragma once

#include <cstdint>
#include <unordered_map>

namespace tiny_cherno {

    namespace profiler {

        struct Profile {
            uint64_t start = 0;
            uint64_t end = 0;

            uint64_t Duration() {
                return end - start;
            }
        };

        void Begin();
        void Begin(const char *frame);
        void End(const char *frame);
        std::unordered_map<const char*, Profile> Finish();
        void PrintResults();
    };

}
