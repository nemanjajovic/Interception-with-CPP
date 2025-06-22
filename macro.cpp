#include <iostream>
#include <windows.h>
#include <string>
#define INTERCEPTION_STATIC
#include "interception.h"

#pragma comment(lib, "interception.lib")

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
    const wchar_t* targetHardwareId = L"HID\\VID_C0F4&PID_07F5";  // Replace with partial ID of your second keyboard

    std::cout << "Creating context..." << std::endl;
    InterceptionContext context = interception_create_context();
    if (!context) {
        std::cerr << "Failed to create context." << std::endl;
        return 1;
    }

    InterceptionDevice targetDevice = 0;
    wchar_t hardwareId[500];

    for (InterceptionDevice device = 1; device <= INTERCEPTION_MAX_DEVICE; ++device) {
        if (interception_is_keyboard(device)) {
            interception_get_hardware_id(context, device, hardwareId, sizeof(hardwareId));
            if (wcsstr(hardwareId, targetHardwareId)) {
                targetDevice = device;
                std::cout << "Found target keyboard on device " << device << std::endl;
                break;
            }
        }
    }

    if (!targetDevice) {
        std::cerr << "Target keyboard not found." << std::endl;
        interception_destroy_context(context);
        return 1;
    }

    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
    std::cout << "Listening for key events from second keyboard...\n" << std::endl;

    InterceptionStroke stroke;
    while (true) {
        InterceptionDevice device = interception_wait(context);
        if (interception_receive(context, device, &stroke, 1) > 0) {
            InterceptionKeyStroke* key = (InterceptionKeyStroke*)&stroke;

            if (device == targetDevice) {
                std::cout << "[SECOND KEYBOARD] Code: 0x" << std::hex << key->code
                          << ", State: " << std::dec << key->state
                          << ", Key: " << GetKeyName(key->code) << std::endl;
                continue;  // Block input from reaching Windows
            }

            interception_send(context, device, &stroke, 1);  // Let other keyboards pass through
        }
    }

    interception_destroy_context(context);
    return 0;
}
