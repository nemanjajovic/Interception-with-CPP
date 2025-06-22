<!-- INSTALLING INTERCEPTION 
Download the latest release from https://github.com/oblitum/Interception/releases
Start CMD as Administrator
Navigate to Interception\command line installer
Run the following command
    install-interception.exe /install
Reboot your PC
After reboot check for keyboard service with sc query keyboard, also keyboard.sys file should be present in System32 folder

COMPILING INTERCEPTION LIBRARIES
Download Interception from https://github.com/oblitum/Interception/tree/master
Run x86 Native Developer Command Line Tools
Navigate to Interception/library and run the following
    cl /c interception.c
    lib interception.obj /OUT:interception.lib
Copy interception.lib and interception.h to your project folder

COMPILING MACRO.CPP
Compile macro.cpp with the below command
Use x86 Native Developer Command Line Tools from Visual Studio and run it as admin
cl /EHsc macro.cpp interception.lib User32.lib /Fe:macro.exe /link /SUBSYSTEM:CONSOLE

No need to touch the files below
Interception.h, interception.lib, macro.lib, macro.obj and macro.exp

Edit macro.cpp and compile it to macro.exe only -->

# 🎯 Interception Setup & Compilation Guide

A complete walkthrough for installing and compiling Interception and `macro.cpp`.

---

## 🛠️ 1. Installing Interception

1. **Download the latest release** from 👉 [Interception Releases](https://github.com/oblitum/Interception/releases)

2. **Run Command Prompt as Administrator**

3. **Navigate** to the folder:  Interception\command line installer

4. **Install Interception** with the command:  
```
install-interception.exe /install
```

5. **Reboot your PC**

6. **Verify installation**:
- Run:
  ```
  sc query keyboard
  ```
- Ensure `keyboard.sys` exists in:
  ```
  C:\Windows\System32
  ```

---

## 🧱 2. Compiling Interception Libraries

1. **Clone the source** from 👉 [Interception Source Code](https://github.com/oblitum/Interception/tree/master)

2. Open **x86 Native Developer Command Prompt** (from Visual Studio 2022)

3. Navigate to: Interception\library

4. Run the following commands:
```
cl /c interception.c
```
```
lib interception.obj /OUT:interception.lib
```

5. **Copy** `interception.lib` and `interception.h` to your project folder

---

## ⚙️ 3. Compiling `macro.cpp`

1. Open **x86 Native Developer Command Prompt** as Administrator

2. From your project folder, compile with:
```
cl /EHsc macro.cpp interception.lib User32.lib /Fe:macro.exe /link /SUBSYSTEM:CONSOLE
```

---

## 🚫 Files to Leave Untouched

> Do **NOT** modify the following files:

- `Interception.h`
- `interception.lib`
- `macro.lib`
- `macro.obj`
- `macro.exp`

Only make changes to `macro.cpp`, then recompile to generate `macro.exe`.

---

## ✅ You're All Set!



