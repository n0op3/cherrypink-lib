#include "profiler.hpp"
#include <chrono>
#include <unordered_map>

namespace tiny_cherno {

    namespace profiler {
        #define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()

        static std::unordered_map<const char*, Profile> s_profiles;

        void Begin() {
            s_profiles.clear();
        }

        void Begin(const char *frame) {
            uint64_t now = NOW;
            s_profiles[frame] = Profile {now, 0};
        }

        void End(const char *frame) {
            uint64_t now = NOW;
            s_profiles[frame].end = now;
        }

        std::unordered_map<const char*, Profile> Finish() {
            auto now = NOW;
            for (auto &[name, profile] : s_profiles) {
                if (profile.end == 0)
                    profile.end = now;
            }

            return s_profiles;
        } 
    }

}
