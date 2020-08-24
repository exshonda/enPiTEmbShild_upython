# LED性能評価

NUCLEO-STM32F401RE用の性能評価プログラム．

## Cプログラム

- cprog/led_shift
  - MicroPythonと同じ処理をするプログラム．

## perf.py

- stm32f401_perf.py

## 評価結果
- STM32F401(84Mhz 11.90ns)

- C言語による計測
 - 0.0357 sec

- led_stm32f401.py による計測

| time.ticks_us() oh | 23.0 usec |
----|---- 
| blink_simple    | 5.418 sec |
| blink_cache1    | 4.705 sec |
| blink_cache2    | 3.061 sec |
| blink_cache3    | 1.980 sec |
| blink_native1   | 3.914 sec |
| blink_native2   | 1.516 sec |
| blink_viper     | 0.253 sec |
| blink_asm       | 0.019 sec |


##mrubyの実行方法
- ビルド
 - [パッケージ](http://forum.mruby.org/download/index212.html)をダウンロード
 - 展開
 - cd ./mruby-2.1.2
 - make

- 実行
 - ./mruby-2.1.2/bin/mruby xxx.rb

