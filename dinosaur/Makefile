
build:
	arduino-cli compile --fqbn UIAP:ch32v:CH32V00x_EVT --output-dir build .
clean:
	rm -rf ./build

upload:
	arduino-cli upload -p /dev/hidraw0 --fqbn UIAP:ch32v:CH32V00x_EVT .

.PHONY: build clean upload
