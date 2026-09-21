#include <UI/windows/about.hpp>
#include <imgui.h>

using namespace ImGui;

void AboutWindow::draw(void* args){
    Begin("About");
        Text(
            "JC8E is a CHIP 8 emulator designed to be used as a educational tool "
            "for teaching computer architecture with an user-friendly unterface."
            "\n"
            "This project was made as my \"capstone project\" in computer engineering."
            "\n"
        );
        TextLinkOpenURL("Github", "https://github.com/JoemanJ/JC8E");
        SameLine();
        Text("diogodelazare@outlook.com");
    End();
}