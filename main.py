import subprocess

try:
    process = subprocess.Popen(
        ["macro.exe"], stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True
    )

    for line in process.stdout:
        key = line.strip()
        print(key)
        # Add logic here...

except KeyboardInterrupt:
    print("\nQuitting the program.")
    process.terminate()  # Gracefully shut down the C++ child process
    process.wait()
