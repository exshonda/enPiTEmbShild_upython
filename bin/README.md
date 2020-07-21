# Build from Source Code

## 環境

- Ubuntu 16.04
  - $ sudo apt-get install libffi-dev
  - $ sudo apt-et install gcc-arm-none-eabi

## ビルド方法

- $ git clone --recurse-submodules https://github.com/micropython/micropython.git
- $ git clone https://github.com/v923z/micropython-ulab.git ulab
- $ cd micropython/mpy-cross
- $ make
- $ cd ../ports/stm32/
- $ make BOARD=NUCLEO_F401RE USER_C_MODULES=../../../ulab all
- $ cd build-NUCLEO_F401RE
- $ arm-none-eabi-objcopy -O binary -S firmware.elf firmware.bin

## インストール方法
- ボードをPCに接続し出てきたドライブに firmware.bin をコピーする．

