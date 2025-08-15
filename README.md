# Arch Update Checker

**Arch Update Checker** is a simple command-line tool to help Arch Linux users check for available package updates, both from the official repositories and the AUR. It also evaluates if any critical system packages need updating and warns you to take a backup before proceeding.

---

## Current Functionalities

- Check for updates on base system packages using `checkupdates`.
- Check for updates on AUR packages using `yay`.
- Highlight critical packages (like `linux`, `glibc`, `systemd`, etc.) that require special attention.
- Log all update information into a timestamped log file.
- Provide color-coded terminal alerts for critical packages.
- Interactive prompts between steps to allow the user to review output.

---

## Future Plans

- Fetch and display relevant news and announcements from the Arch Wiki / Arch Linux news feed.
- Possibly add a GUI front-end to enhance usability (this is my first GUI package — any advice or feedback would be greatly appreciated!).

---

## About This Project

This is my first time building an Arch Linux package and working on a GUI project. I’m uploading this program to the AUR once it’s polished and stable enough. I welcome any tips, suggestions, or contributions from the community to help improve the tool!

---
## Installation

You can install **arch_update_cli_tool** from the AUR using your preferred AUR helper or by cloning the repository manually.

### Using an AUR helper

With **yay**:

```bash
yay -S arch_update_cli_tool
```

With **paru**:

```bash
paru -S arch_update_cli_tool
```

### Manual installation (Download & Build)

Clone the repository and build the package manually:
```bash
git clone https://aur.archlinux.org/arch_update_cli_tool.git
cd arch_update_cli_tool
makepkg -si
```
This will download the package from the AUR, compile the tool, and install it on your system.

## Usage

Once installed, simply run:
```bash
arch-update-checker
```

The log will be created in the directory where the command is executed and will be updated on subsequent runs.
