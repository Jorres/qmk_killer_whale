package main

import (
	"bufio"
	"fmt"
	"log"
	"os/exec"
	"strings"
	"time"

	"github.com/sstallion/go-hid"
)

const (
	vendorID  = 0x8884
	productID = 0x0899
	usagePage = 0xFF60
	usage     = 0x61

	cmdLayoutUpdate = 0x01
	reportSize      = 32

	reconnectInterval = 500 * time.Millisecond
)

// parseLayout checks if a gsettings mru-sources line indicates Russian layout.
// Returns 1 for Russian, 0 for everything else.
func parseLayout(line string) uint8 {
	if idx := strings.Index(line, "["); idx >= 0 {
		after := line[idx:]
		if strings.HasPrefix(after, "[('xkb', 'ru')") {
			return 1
		}
	}
	return 0
}

func getCurrentLayout() (uint8, error) {
	out, err := exec.Command("gsettings", "get", "org.gnome.desktop.input-sources", "mru-sources").Output()
	if err != nil {
		return 0, fmt.Errorf("gsettings: %w", err)
	}
	return parseLayout(string(out)), nil
}

func openDevice() (*hid.Device, error) {
	var dev *hid.Device
	hid.Enumerate(vendorID, productID, func(info *hid.DeviceInfo) error {
		if info.UsagePage == usagePage && info.Usage == usage {
			d, err := hid.OpenPath(info.Path)
			if err == nil {
				dev = d
			}
		}
		return nil
	})

	if dev == nil {
		return nil, fmt.Errorf("keyboard not found")
	}

	return dev, nil
}

func sendLayout(dev *hid.Device, layout uint8) error {
	// hidapi requires report ID as first byte; QMK Raw HID uses report ID 0
	buf := make([]byte, reportSize+1)
	buf[0] = 0x00 // report ID
	buf[1] = cmdLayoutUpdate
	buf[2] = layout

	_, err := dev.Write(buf)
	return err
}

// waitForDevice blocks until the keyboard is connected, retrying every reconnectInterval.
func waitForDevice() *hid.Device {
	for {
		dev, err := openDevice()
		if err == nil {
			return dev
		}
		time.Sleep(reconnectInterval)
	}
}

// layoutEvents starts gsettings monitor and sends parsed layout values on the channel.
// Blocks forever; meant to run in a goroutine.
func layoutEvents(ch chan<- uint8) {
	cmd := exec.Command("gsettings", "monitor", "org.gnome.desktop.input-sources", "mru-sources")
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		log.Fatalf("Failed to create pipe: %v", err)
	}
	if err := cmd.Start(); err != nil {
		log.Fatalf("Failed to start gsettings monitor: %v", err)
	}

	scanner := bufio.NewScanner(stdout)
	for scanner.Scan() {
		ch <- parseLayout(scanner.Text())
	}
	log.Fatal("gsettings monitor exited unexpectedly")
}

func main() {
	log.Println("lang_sync: starting")

	if err := hid.Init(); err != nil {
		log.Fatalf("hid init: %v", err)
	}
	defer hid.Exit()

	// Start listening for layout events in background
	layoutCh := make(chan uint8, 8)
	go layoutEvents(layoutCh)

	// Read initial layout
	layout, err := getCurrentLayout()
	if err != nil {
		log.Printf("Warning: could not read initial layout: %v", err)
	}

	var dev *hid.Device

	// Connect to keyboard (waits if not present)
	log.Println("lang_sync: waiting for keyboard...")
	dev = waitForDevice()
	log.Println("lang_sync: keyboard connected")

	// Send initial layout
	if err := sendLayout(dev, layout); err != nil {
		log.Printf("Warning: could not send initial layout: %v", err)
	} else {
		log.Printf("lang_sync: initial layout = %d", layout)
	}

	// Main loop: react to layout changes, reconnect on device errors
	for newLayout := range layoutCh {
		if newLayout == layout {
			continue
		}

		if err := sendLayout(dev, newLayout); err != nil {
			log.Printf("Device error: %v — reconnecting...", err)
			dev.Close()
			dev = waitForDevice()
			log.Println("lang_sync: keyboard reconnected")
			// Resend after reconnect
			if err := sendLayout(dev, newLayout); err != nil {
				log.Printf("Error after reconnect: %v", err)
				continue
			}
		}

		log.Printf("lang_sync: layout changed %d -> %d", layout, newLayout)
		layout = newLayout
	}
}
