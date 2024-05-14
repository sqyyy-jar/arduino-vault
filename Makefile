setup:
	arduino-cli core install arduino:avr
	arduino-cli lib install "SD"
	arduino-cli lib install "Adafruit SSD1306"
	arduino-cli lib install "Keypad"

build:
	arduino-cli compile -b arduino:avr:mega Vault

run:
	arduino-cli compile -u -p /dev/ttyACM0 -b arduino:avr:mega Vault
	arduino-cli monitor -p /dev/ttyACM0

.PHONY: setup build run

.DEFAULT_GOAL := build
