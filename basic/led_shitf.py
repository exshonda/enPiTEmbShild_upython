from machine import Pin

def led_init():
  global led1,led2,led3,led4
  led1 = Pin('PA8', Pin.OUT, Pin.PULL_NONE)
  led2 = Pin('PA7', Pin.OUT, Pin.PULL_NONE)
  led3 = Pin('PA6', Pin.OUT, Pin.PULL_NONE)
  led4 = Pin('PA5', Pin.OUT, Pin.PULL_NONE)

def led_out(led_data):
  if (led_data & 0x01) == 0:
    led1.low()
  else:
    led1.high()
  if (led_data & 0x02) == 0:
    led2.low()
  else:
    led2.high()
  if (led_data & 0x04) == 0:
    led3.low()
  else:
    led3.high()
  if (led_data & 0x08) == 0:
    led4.low()
  else:
    led4.high()

def busy_wait():
  for _ in range(100000):
    pass

def main():
  led_init()
  while True:
    led_out(0x01)
    busy_wait()
    led_out(0x02)
    busy_wait()
    led_out(0x04)
    busy_wait()
    led_out(0x08)
    busy_wait()

if __name__ == '__main__':
  main()
