#include <windows.h>
#include <cstdint>

using ScriptMain = void(*)();
using KeyboardHandler = void(*)(DWORD, WORD, BYTE, BOOL, BOOL, BOOL, BOOL);

extern "C" {
    __declspec(dllimport) void scriptRegister(HMODULE module, ScriptMain main);
    __declspec(dllimport) void scriptWait(DWORD time);
    __declspec(dllimport) void keyboardHandlerRegister(KeyboardHandler handler);
    __declspec(dllimport) void nativeInit(std::uint64_t hash);
    __declspec(dllimport) void nativePush64(std::uint64_t value);
    __declspec(dllimport) std::uint64_t* nativeCall();
}

namespace {
    volatile LONG g_toggleRequested = 0;
    bool g_godMode = false;

    constexpr std::uint64_t HASH_PLAYER_ID = 0x4F8644AF03D0E0D6ULL;
    constexpr std::uint64_t HASH_SET_PLAYER_INVINCIBLE = 0x239528EACDC3E7DEULL;
    constexpr std::uint64_t HASH_BEGIN_TEXT_COMMAND_THEFEED_POST = 0x202709F4C58A0424ULL;
    constexpr std::uint64_t HASH_ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME = 0x6C188BE134E074AAULL;
    constexpr std::uint64_t HASH_END_TEXT_COMMAND_THEFEED_POST_TICKER = 0x2ED7843F8F801023ULL;

    std::uint64_t callNative0(std::uint64_t hash) {
        nativeInit(hash);
        return *nativeCall();
    }

    void setGodMode(bool enabled) {
        const std::uint64_t player = callNative0(HASH_PLAYER_ID);
        nativeInit(HASH_SET_PLAYER_INVINCIBLE);
        nativePush64(player);
        nativePush64(enabled ? 1ULL : 0ULL);
        nativeCall();
    }

    void showNotification(const char* text) {
        nativeInit(HASH_BEGIN_TEXT_COMMAND_THEFEED_POST);
        nativePush64(reinterpret_cast<std::uint64_t>("STRING"));
        nativeCall();

        nativeInit(HASH_ADD_TEXT_COMPONENT_SUBSTRING_PLAYER_NAME);
        nativePush64(reinterpret_cast<std::uint64_t>(text));
        nativeCall();

        nativeInit(HASH_END_TEXT_COMMAND_THEFEED_POST_TICKER);
        nativePush64(0ULL);
        nativePush64(1ULL);
        nativeCall();
    }

    void onKeyboard(
        DWORD key,
        WORD,
        BYTE,
        BOOL,
        BOOL,
        BOOL,
        BOOL isKeyUp
    ) {
        // G is the reliable Mac/Wine key. F5 remains available as a second option.
        if (isKeyUp && (key == 'G' || key == VK_F5)) {
            InterlockedExchange(&g_toggleRequested, 1);
        }
    }

    void ScriptMain() {
        keyboardHandlerRegister(onKeyboard);

        while (true) {
            if (InterlockedExchange(&g_toggleRequested, 0) != 0) {
                g_godMode = !g_godMode;
                setGodMode(g_godMode);
                showNotification(g_godMode ? "God Mode: ON" : "God Mode: OFF");
            }

            // Reapply continuously because missions and character switches can reset the flag.
            if (g_godMode) {
                setGodMode(true);
            }

            scriptWait(0);
        }
    }
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(module);
        scriptRegister(module, ScriptMain);
    }
    return TRUE;
}
