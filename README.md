

```
brew install platformio

npm install
gulp
```


## Testing Web portal using local web server

`gulp connect`

## Stack trace decode

`java -jar tools/EspStackTraceDecoder.jar ~/.platformio/packages/toolchain-xtensa/bin/xtensa-lx106-elf-addr2line .pioenvs/nodemcuv2/firmware.elf stack.txt`

## Serial

`pio device monitor --port /dev/cu.wchusbserial1410 --baud 115200`

## Flash

`esptool.py --port /dev/cu.wchusbserial1410 --baud 115200 write_flash --flash_size=detect -fm dio 0 .pioenvs/nodemcuv2/firmware.bin`
