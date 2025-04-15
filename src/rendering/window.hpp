#pragma once

#include <utility>
#include "rendering/context.hpp"

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
            int Width() const;
            int Height() const;
            void SetWidth(unsigned int width);
            void SetHeight(unsigned int height);
            bool IsResizable() const;
            void SetResizable(bool resizable);
            bool IsVisible() const;
            void Show();
            void Hide();
            bool ShouldClose() const;
            void Close();
            const char* Title() const;
            void SetTitle(const char* title);
            int GetAttribute(int attribute) const;
            void SetAttribute(int attribute, int value);
            RenderingContext *Context();
            void SetRenderingContext(RenderingContext *renderingContext);
            GLFWwindow *Handle() const;
        private:
            GLFWwindow *m_handle;
            RenderingContext *m_renderingContext;
    };

} // namespace tiny_cherno
