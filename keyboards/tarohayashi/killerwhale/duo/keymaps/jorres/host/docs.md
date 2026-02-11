# Build the binary

cd ~/hobbies/qmk_killer_whale/keyboards/tarohayashi/killerwhale/duo/keymaps/jorres/host
go build -o lang_sync .

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
