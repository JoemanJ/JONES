class IWindow{
    public:
        /*
        Draws the DearImgui window.

        Equivalent to drawing manually using:
        ImGui::Begin();
            ...
        Imgui::End();
        */
        virtual void draw(void* args) = 0;
};