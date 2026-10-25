#include "hacks.h"
#include "gui.h"
#include "globals.h"

#include <thread>

void hacks::VisualThread(const Memory& mem) noexcept 
{
    while (gui::isRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        const auto localPlayer = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwLocalPlayer);
        if (!localPlayer)
            continue;

        const auto glowObjectManager = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwGlowObjectManager);
        if (!glowObjectManager)
            continue;

        const auto localTeam = mem.Read<std::int32_t>(localPlayer + offsets::m_iTeamNum);
        if (!localTeam)
            continue;

        for (auto i = 0; i < 32; ++i)
        {
            const auto player = mem.Read<std::uintptr_t>(globals::clientAddress + offsets::dwEntityList + i * 0x10);
            if (!player)
                continue;

            const auto team = mem.Read<std::int32_t>(player + offsets::m_iTeamNum);
            if (team == localTeam)
                continue;

            const auto lifeState = mem.Read<std::int32_t>(player + offsets::m_lifeState);
            if (lifeState != 0)
                continue;

            if (globals::glow)
            {
                const auto glowIndex = mem.Read<std::int32_t>(player + offsets::m_iGlowIndex);

                mem.Write(glowObjectManager + (glowIndex * 0x38) + 0xB, globals::glowColor[0]); // red
                mem.Write(glowObjectManager + (glowIndex * 0x38) + 0xC, globals::glowColor[1]); // green
                mem.Write(glowObjectManager + (glowIndex * 0x38) + 0x10, globals::glowColor[2]);  // blue
                mem.Write(glowObjectManager + (glowIndex * 0x38) + 0x14, globals::glowColor[3]);  // alpha

                mem.Write(glowObjectManager + (glowIndex * 0x38) + 0x28, true);
                mem.Write(glowObjectManager + (glowIndex * 0x38) + 0x29, false);
            }

            if (globals::radar)
                mem.Write(player + offsets::m_bSpotted, true);
        }
    }
}