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
    std::cout << "Creating context..." << std::endl;
    InterceptionContext context = interception_create_context();
    if (!context) {
        std::cerr << "Failed to create context." << std::endl;
        return 1;
    }

    // Detect all keyboard devices and collect hardware IDs
    std::vector<std::pair<InterceptionDevice, std::wstring>> keyboards;

    wchar_t buffer[500];
    for (InterceptionDevice device = 1; device <= INTERCEPTION_MAX_DEVICE; ++device) {
        if (interception_is_keyboard(device)) {
            interception_get_hardware_id(context, device, buffer, sizeof(buffer));
            keyboards.emplace_back(device, buffer);
        }
    }

    if (keyboards.empty()) {
        std::cerr << "No keyboard devices found." << std::endl;
        interception_destroy_context(context);
        return 1;
    }

    // Sort USB devices (starting with "HID\\VID_") to the top
    std::sort(keyboards.begin(), keyboards.end(),
        [](const auto& a, const auto& b) {
            bool a_is_usb = wcsstr(a.second.c_str(), L"HID\\VID_") != nullptr;
            bool b_is_usb = wcsstr(b.second.c_str(), L"HID\\VID_") != nullptr;
            return a_is_usb > b_is_usb;
        });

    // Select the first USB keyboard automatically
    InterceptionDevice targetDevice = keyboards[0].first;
    std::wcout << L"Selected USB device: " << keyboards[0].second << L" (device " << targetDevice << L")\n";

    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);

    InterceptionStroke stroke;
    while (true) {
        InterceptionDevice device = interception_wait(context);
        if (interception_receive(context, device, &stroke, 1) > 0) {
            InterceptionKeyStroke* key = reinterpret_cast<InterceptionKeyStroke*>(&stroke);

            if (device == targetDevice && key->state == 0) { // Fire only on key down
            std::cout << GetKeyName(key->code) << std::endl;
            continue;  // Block from OS
            }

            interception_send(context, device, &stroke, 1); // Let others through
        }
    }

    interception_destroy_context(context);
    return 0;
}
