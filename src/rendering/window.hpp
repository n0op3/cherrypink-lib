#pragma once

#include <utility>

struct GLFWwindow;

namespace tiny_cherno {

    struct WindowParameters {
        const char *title;
        unsigned int width;
        unsigned int height;
        bool resizable;
    };

    class Window {
        public:
            Window(WindowParameters params);
            std::pair<int, int> Size() const;
            void Update();
            int Width() const;
            int Height() const;
            void SetWidth(unsigned int width);
            void SetHeight(unsigned int height);
            bool IsResizable() const;
            void SetResizable(bool resizable);
            bool IsVisible() const;
            bool ShouldClose() const;
            void Close();
            const char* Title() const;
            void SetTitle(const char* title);
            int GetAttribute(int attribute) const;
            void SetAttribute(int attribute, int value);
            GLFWwindow *Handle() const;
        private:
            GLFWwindow *m_handle;
    };

} // namespace tiny_cherno
