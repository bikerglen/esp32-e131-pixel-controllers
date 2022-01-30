help:
	@echo "valid targets:"
	@echo "  make compile-esp32-gateway-1chlamp && make upload-esp32-gateway-1chlamp"
	@echo "  make compile-wesp32-poe-mini1ch && make upload-wesp32-poe-mini1ch"
	@echo "  make compile-wesp32-poe-full4ch && make upload-wesp32-poe-full4ch"
	@echo "  make compile-esp32-poe-full4ch && make upload-esp32-poe-full4ch"


compile-esp32-gateway-1chlamp: 
	arduino-cli compile \
		--fqbn esp32:esp32:esp32-gateway:Revision=RevF \
		--build-property "compiler.cpp.extra_flags=-DUNIVERSES=1 -DUPIN1=12" \
		esp32-e131-pixels.ino

upload-esp32-gateway-1chlamp:
	arduino-cli upload \
		-p /dev/ttyUSB0 \
		--fqbn esp32:esp32:esp32-gateway:Revision=RevF \
		esp32-e131-pixels.ino


compile-wesp32-poe-mini1ch: 
	arduino-cli compile \
		--fqbn esp32:esp32:wesp32 \
		--build-property "compiler.cpp.extra_flags=-DUNIVERSES=1 -DUPIN1=15" \
		esp32-e131-pixels.ino

upload-wesp32-poe-mini1ch:
	arduino-cli upload \
		-p /dev/ttyUSB0 \
		--fqbn esp32:esp32:wesp32 \
		esp32-e131-pixels.ino


compile-wesp32-poe-full4ch: 
	arduino-cli compile \
		--fqbn esp32:esp32:wesp32 \
		--build-property "compiler.cpp.extra_flags=-DUNIVERSES=4 -DUPIN1=15 -DUPIN2=12 -DUPIN3=13 -DUPIN4=14" \
		esp32-e131-pixels.ino

upload-wesp32-poe-full4ch:
	arduino-cli upload \
		-p /dev/ttyUSB0 \
		--fqbn esp32:esp32:wesp32 \
		esp32-e131-pixels.ino


compile-esp32-poe-full4ch: 
	arduino-cli compile \
		--fqbn esp32:esp32:esp32-poe-iso \
		--build-property "compiler.cpp.extra_flags=-DUNIVERSES=4 -DUPIN1=13 -DUPIN2=14 -DUPIN3=15 -DUPIN4=16" \
		esp32-e131-pixels.ino

upload-esp32-poe-full4ch:
	arduino-cli upload \
		-p /dev/ttyUSB0 \
		--fqbn esp32:esp32:esp32-poe-iso \
		esp32-e131-pixels.ino


