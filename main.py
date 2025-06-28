import subprocess


# Example function
def example_function():
    print("You pressed W")


# Add python logic here
key_actions = {
    "q": lambda: print("You pressed Q"),
    "w": example_function,
    # Add more keys and actions here
}

try:
    process = subprocess.Popen(
        ["macro.exe"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )
    print("Macro.exe started!")
    print("Press any key to register a keyboard as a macro device...")

    for line in process.stdout:
        key = line.strip().lower()
        action = key_actions.get(key)

        if action:
            action()
        else:
            print(key)

except KeyboardInterrupt:
    print("Quitting Macro.exe...")
    process.terminate()  # Gracefully shut down the C++ child process
    process.wait()
    print("Macro.exe sucessfully stopped!")
