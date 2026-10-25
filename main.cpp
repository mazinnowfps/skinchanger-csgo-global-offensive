#include "gui.h"
#include "memory.h"
#include "globals.h"
#include "hacks.h"

#include <thread>

int APIENTRY wWinMain(HINSTANCE instance, HINSTANCE previousInstance, PWSTR arguments, int commandShow) {
    // Configure settings before creating window
    gui::isDPIAware = true;
    gui::vSyncEnabled = true;

    Memory mem{ "csgo.exe" };

    globals::clientAddress = mem.GetModuleAddress("client.dll");

    std::thread (hacks::VisualThread, mem).detach();

    // create gui
    gui::CreateHWindow(L"Cheat Menu");
    gui::CreateDevice();
    gui::CreateImGui();

    while (gui::isRunning) {
        gui::BeginRender();
        gui::Render();
        gui::EndRender();

        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }

    // destroy gui
    gui::DestroyImGui();
    gui::DestroyDevice();
    gui::DestroyHWindow();

    return EXIT_SUCCESS;
}
