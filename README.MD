# 🎯 Interception Setup & Compilation Guide

A complete walkthrough for installing and compiling Interception and `macro.cpp`.

---

## 🛠️ 1. Installing Interception

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

## 🧱 2. Compiling Interception Libraries

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

## ⚙️ 3. Compiling `macro.cpp`

1. Open **x86 Native Developer Command Prompt** as Administrator

2. From your project folder, compile with:
```
build.bat
```

---

## 🧠 4. Running the Program

Run everything through your Python interface for clean control and logging:

```bash
python main.py
```

This will:

- Launch `macro.exe`
- Read key events from your USB keyboard
- Let you trigger actions directly from `main.py`


## ✅ You're All Set!



