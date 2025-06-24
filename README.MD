## About This Project

This project turns your secondary keyboard into a powerful macro engine.

By wrapping the low-level Interception driver with C++, we isolate input from a specific USB keyboard — treating it independently from your main input devices. Each key press is detected and streamed live into a Python script, where you can assign custom actions, automations, or scripts to individual keys.

Whether you want to launch applications, control media, trigger hotkeys, or streamline your workflow — this setup transforms an ordinary keyboard into your own programmable control surface.

---

## Prerequisites

Before getting started, make sure you have:

- **Visual Studio 2022** with the *Desktop development with C++* workload  
- **Python** installed and added to your system PATH  
- A **secondary keyboard** (preferably USB) to act as your macro trigger device  
- Basic knowledge of **C++ compilation** and **Python scripting**  
- Administrator privileges (required for installing the Interception driver)

---

## Project Goals

- Isolate input from a dedicated USB keyboard
- Use C++ with the Interception driver to capture key events at the system level
- Stream key events into Python in real time
- Enable powerful desktop automation, hotkeys, macros, and scripting
- Maintain separation from your primary keyboard and input devices

---

## Limitations

- **Windows only** — Interception is a Windows-specific driver and won’t run on macOS or Linux  
- **Driver required** — Interception installs a low-level keyboard filter that needs admin rights and a system reboot  
- **Touchpad interference** — Some systems may see conflicts between Interception and built-in input devices (like laptop touchpads)  
- **Single device focus** — Currently optimized for one secondary keyboard; multi-keyboard profiles or hot-swapping not yet implemented  
- **Requires cleanup** — Uninstalling the driver improperly may leave your input devices unresponsive

---

# Installation and compilation guide

---

## Installing Interception Driver

1. **Run Command Prompt as Administrator**

2. **Install Interception** with the command:  
```
./install-interception.exe /install
```

3. **Reboot your PC**

4. **Verify installation**:
- Run:
  ```
  sc query keyboard
  ```
- Ensure `keyboard.sys` exists in:
  ```
  C:\Windows\System32
  ```

---

## Compiling Interception Libraries

1. Open **x86 Native Developer Command Prompt** (from Visual Studio 2022)

2. Navigate to: ./library

3. Run the following commands:
```
cl /c interception.c
```
```
lib interception.obj /OUT:interception.lib
```

---

## Compiling `macro.cpp`

1. Open **x86 Native Developer Command Prompt** as Administrator

2. From your project folder, compile with:
```
build.bat
```

---

## Running the Program

Run everything through your Python interface for clean control and logging:

```bash
python main.py
```

This will:

- Launch `macro.exe`
- Read key events from your USB keyboard
- Let you trigger actions directly from `main.py`

---

## You're All Set!

Add your custom logic to `main.py` — each keypress from your USB keyboard will trigger it in real time.

With that, you’ve transformed your keyboard into a dedicated macro device ready to automate anything you want.