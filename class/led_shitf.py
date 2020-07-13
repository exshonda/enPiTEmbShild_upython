from machine import Pin

class LED:
  LED1 = 1
  LED2 = 2
  LED3 = 4
  LED4 = 8
  def __init__(self):
    self.led1 = Pin('PA8', Pin.OUT, Pin.PULL_NONE)
    self.led2 = Pin('PA7', Pin.OUT, Pin.PULL_NONE)
    self.led3 = Pin('PA6', Pin.OUT, Pin.PULL_NONE)
    self.led4 = Pin('PA5', Pin.OUT, Pin.PULL_NONE)

  def out_bdigit(self, led_data):
    if (led_data & self.LED1) == 0:
      self.led1.low()
    else:
      self.led1.high()
    if (led_data & self.LED2) == 0:
      self.led2.low()
    else:
      self.led2.high()
    if (led_data & self.LED3) == 0:
      self.led3.low()
    else:
      self.led3.high()
    if (led_data & self.LED4) == 0:
      self.led4.low()
    else:
      self.led4.high()

def busy_wait():
  for _ in range(100000):
    pass

def main():
  led = LED()
  while True:
    led.out_bdigit(0x01)
    busy_wait()
    led.out_bdigit(0x02)
    busy_wait()
    led.out_bdigit(0x04)
    busy_wait()
    led.out_bdigit(0x08)
    busy_wait()

if __name__ == '__main__':
  main()
