#include <iostream>
#include <windows.h>
#include <string>
#define INTERCEPTION_STATIC
#include "interception.h"

// Link against the Interception static library
#pragma comment(lib, "interception.lib")

// Converts a scan code into a human-readable key name
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
    // Hardware ID string identifying the second keyboard (partial match is okay)
    const wchar_t* targetHardwareId = L"HID\\VID_C0F4&PID_07F5";

    std::cout << "Creating context..." << std::endl;
    InterceptionContext context = interception_create_context();
    if (!context) {
        std::cerr << "Failed to create context." << std::endl;
        return 1;
    }

    InterceptionDevice targetDevice = 0;
    wchar_t hardwareId[500];

    // Search all devices to find the one matching the second keyboard
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

    // Exit if the second keyboard wasn't found
    if (!targetDevice) {
        std::cerr << "Target keyboard not found." << std::endl;
        interception_destroy_context(context);
        return 1;
    }

    // Enable key press/release events for all keyboards
    interception_set_filter(context, interception_is_keyboard, INTERCEPTION_FILTER_KEY_DOWN | INTERCEPTION_FILTER_KEY_UP);
    std::cout << "Listening for key events from second keyboard...\n" << std::endl;

    InterceptionStroke stroke;

    // Main loop: wait for key events and process them
    while (true) {
        InterceptionDevice device = interception_wait(context);

        // If a stroke was successfully received
        if (interception_receive(context, device, &stroke, 1) > 0) {
            InterceptionKeyStroke* key = (InterceptionKeyStroke*)&stroke;
            
            // If it's from the second keyboard, print it and block it
            if (device == targetDevice) {
                std::cout << "[SECOND KEYBOARD] Code: 0x" << std::hex << key->code
                          << ", State: " << std::dec << key->state
                          << ", Key: " << GetKeyName(key->code) << std::endl;
                continue;  // Block input from reaching Windows
            }

            // Otherwise, pass input from other keyboards through
            interception_send(context, device, &stroke, 1);
        }
    }

    interception_destroy_context(context);
    return 0;
}
