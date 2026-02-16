# Build

cd ~/hobbies/qmk_killer_whale/keyboards/tarohayashi/killerwhale/duo/keymaps/jorres/host
make

This builds two binaries:
- `lang_sync` — Go daemon that sends layout changes to the keyboard via Raw HID
- `xkb_monitor` — small C helper that monitors X11 XKB layout group changes

## Build dependencies

**Go daemon** (`lang_sync`): just Go.

**X11 monitor** (`xkb_monitor`): needs gcc and X11 dev headers:

    sudo apt install libx11-dev libxkbfile-dev

Only needed when running on X11. On Wayland, `xkb_monitor` is not used (the daemon
falls back to `gsettings monitor`).

## How it works

The daemon detects `$XDG_SESSION_TYPE` at startup:
- **Wayland**: monitors `gsettings org.gnome.desktop.input-sources mru-sources`
- **X11**: spawns the `xkb_monitor` helper (must be in the same directory as `lang_sync`)

`xkb_monitor` listens for XKB StateNotify events and prints the layout name
(e.g. "us", "ru") to stdout on each switch. The Go daemon reads these lines and
sends the corresponding layout index to the keyboard over Raw HID.

# Symlink the service file (so edits in the repo take effect)

mkdir -p ~/.config/systemd/user
ln -sf "$(pwd)/lang-sync.service" ~/.config/systemd/user/lang-sync.service

# Enable and start

systemctl --user daemon-reload
systemctl --user enable --now lang-sync

To monitor later:

systemctl --user status lang-sync # quick health check
journalctl --user -u lang-sync -f # tail live logs
journalctl --user -u lang-sync --since "1 hour ago" # recent history

The service restarts automatically after 3 seconds if it crashes (e.g. keyboard
disconnected). %h in the service file expands to your home directory, so it's portable.

One thing to note: the daemon needs HID device access. If you get a permission error,
you'll need a udev rule:

# /etc/udev/rules.d/50-killerwhale.rules

SUBSYSTEM=="hidraw", ATTRS{idVendor}=="8884", ATTRS{idProduct}=="0899", MODE="0660",
TAG+="uaccess"

Then sudo udevadm control --reload-rules && sudo udevadm trigger.
