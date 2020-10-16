# Build from Source Code

## 環境
- 参考URL
  - [オフィシャル](https://github.com/micropython/micropython/wiki/Getting-Started)

- Ubuntu 16.04
  - sudo apt-get install libffi-dev
  - sudo apt-get install gcc-arm-none-eabi

## ビルド方法(F401RE)

- git clone --recurse-submodules https://github.com/micropython/micropython.git
- git clone https://github.com/v923z/micropython-ulab.git ulab -b 0.51.1
- cd micropython/mpy-cross
- make
- cd ../ports/stm32/
- make BOARD=NUCLEO_F401RE USER_C_MODULES=../../../ulab all
- cd build-NUCLEO_F401RE
- arm-none-eabi-objcopy -O binary -S firmware.elf firmware.bin

## インストール方法
- NUCLEO系
  - ボードをPCに接続し出てきたドライブに firmware.bin をコピーする．
- Pyboard
  - [ここ](https://github.com/micropython/micropython/wiki/Pyboard-Firmware-Update)を参考にDFUモードにする．
  - STM32CubeProgrammerで接続して.binを書き込む．

## ビルド方法(UNIX)

- cd ./micropython/ports/unix
- make axtls
- make

## 実行方法(UNIX)
- cd ./micropython/ports/unix
- ./micropython