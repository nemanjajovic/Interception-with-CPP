#include <iostream>
#include <windows.h>
#include <string>
#include <vector>
#include <algorithm>
#define INTERCEPTION_STATIC
#include "library/interception.h"

#pragma comment(lib, "User32.lib")

std::string GetKeyName(unsigned short code) {
    UINT scan = code & 0xFF;
    if (code & 0xE000) scan |= 0xE000;

    UINT vk = MapVirtualKey(scan, MAPVK_VSC_TO_VK);
    if (!vk) return "[Unknown]";

    char name[128] = {};
    if (GetKeyNameTextA(scan << 16, name, sizeof(name)) > 0)
        return std::string(name);

    return "[Unknown]";
}

int main() {
    InterceptionContext context = interception_create_context();
    if (!context) return 1;

    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

    InterceptionDevice macroDevice = 0;
    InterceptionStroke stroke;

    while (true) {
        InterceptionDevice device = interception_wait(context);
        if (interception_receive(context, device, &stroke, 1) > 0) {
            InterceptionKeyStroke* key = reinterpret_cast<InterceptionKeyStroke*>(&stroke);

            if (key->state == 0) { // Key down
                if (macroDevice == 0 && interception_is_keyboard(device)) {
                    macroDevice = device;
                    std::cout << "Macro keyboard registered on device " << macroDevice << std::endl;
                    continue; // Don’t send to OS
                }

                if (device == macroDevice) {
                    std::cout << GetKeyName(key->code) << std::endl;
                    continue; // Swallow macro key
                }
            }

            // For non-macro input devices, let keystroke pass through
            interception_send(context, device, &stroke, 1);
        }
    }

    interception_destroy_context(context);
    return 0;
}

