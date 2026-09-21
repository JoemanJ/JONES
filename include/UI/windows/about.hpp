#include "window.hpp"
#include <cstddef>

class AboutWindow: IWindow{
    public: 
        /*
        Draws the DearImgui window.

        Equivalent to drawing manually using:
        ImGui::Begin();
            ...
        Imgui::End();
        */
        void draw(void* args = NULL);
};