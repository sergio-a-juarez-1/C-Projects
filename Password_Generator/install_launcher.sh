#!/bin/bash
# Script to install the Password Generator to the Ubuntu Application Menu

# 1. Grab absolute paths of the active workspace folder configuration
APP_DIR=$(pwd)
BINARY_PATH="$APP_DIR/gui_password"
DESKTOP_FILE="$HOME/.local/share/applications/c-password-generator.desktop"

# 2. Safety Check: Verify that the compiler binary target exists
if [ ! -f "$BINARY_PATH" ]; then
    echo "[-] Error: Executable target 'gui_password' not found."
    echo "    Please run 'make' to compile the program before installing."
    exit 1
fi

echo "[+] Staging system definitions for path: $BINARY_PATH"

# 3. Stream structural key definitions directly down to the local desk array
cat << EOF > "$DESKTOP_FILE"
[Desktop Entry]
Type=Application
Version=1.0
Name=C-Password Generator
Comment=Zero-Dependency Ultra-Fast Entropy Key Generator
Exec=$BINARY_PATH
Path=$APP_DIR
Terminal=false
Categories=Utility;Development;Security;
Keywords=password;generator;security;c;
Icon=security-low
StartupNotify=true
EOF

# 4. Modify access permissions to make the shortcut executable
chmod +x "$DESKTOP_FILE"

# 5. Flush the system desktop window database cache to register changes
if command -v update-desktop-database &> /dev/null; then
    update-desktop-database "$HOME/.local/share/applications"
fi

echo "[+] Success! The application shortcut has been successfully compiled."
echo "    You can now find 'C-Password Generator' inside your Ubuntu Search Bar."
