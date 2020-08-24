# MicroPython for enPiTEmb Shild

## 環境構築

- Windows PC
  - WSL をインストール
  - pythonをインストール
  - ~/.local/bin/ にパスを通す

- dfuSuDemo
  - [STのサイト](https://www.st.com/en/development-tools/stsw-stm32080.html) からダウンロード

- Board(Nucleo STM32F401RE)
  - [DFU](https://micropython.org/download/stm32/)のダウンロード
  - Dfu file manager で.hexに変換
  - WSL で以下のコマンドで.binに変換
    - objcopy -I ihex hoge.hex -O binary micropython.bin
  - PCとUSBケーブルで接続
  - フォルダが追加されるので，micropython.bin をコピー

- ampy 
  - インストール
    - sudo pip install adafruit-ampy
  - 環境変数
    - export AMPY_PORT=/dev/ttySxx (xx内はcomポートの番号)

- mpy-cross
  - 以下のコマンドでビルド
    - $ git clone --recursive https://github.com/micropython/micropython.git
    - $ make -C micropython/mpy-cross
  - 次のコマンドをパスが通っている場所にコピー
    - micropython/mpy-cross/mpy-cross 

## 実行等

- 実行
  - ampy run xx.py

- ファイルのコピー
  - ampy put xx.py

## UNIX向けのビルド



## フォルダ

- basic
  - machineのクラスを使用したバージョン
- basic_pyb
  - pybクラスを使用したバージョン
- class
  - basicをクラス化したバージョン
- module
  - classをモジュール化したバージョン
- bin
  - NUCLEO_F401RE_ulab.bin
   - コミット番号 27767aa のmicropython + ulab 0.51.1 で作成したNUCLEO_F401RE用のバイナリファイル．
