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

class SW:
  SW_OFF = 0
  SW_ON  = 1
  SW1 = 1
  SW2 = 2
  SW3 = 4
  SW4 = 8

  def __init__(self):
    self.sw1 = Pin('PB3', Pin.IN, Pin.PULL_NONE)
    self.sw2 = Pin('PB4', Pin.IN, Pin.PULL_NONE)
    self.sw3 = Pin('PB5', Pin.IN, Pin.PULL_NONE)
    self.sw4 = Pin('PB6', Pin.IN, Pin.PULL_NONE)

  def sense(self):
    reg = 0
    if self.sw1.value() == 1:
      reg = self.SW1
    if self.sw2.value() == 1:
      reg |= self.SW2
    if self.sw3.value() == 1:
      reg |= self.SW3
    if self.sw4.value() == 1:
      reg |= self.SW4
    return reg

  def sense_sw1(self):
    if self.sw1.value() == 1:
      return self.SW_ON
    else:
      return self.SW_OFF

  def sense_sw2(self):
    if self.sw2.value() == 1:
      return self.SW_ON
    else:
      return self.SW_OFF

def busy_wait():
  for _ in range(100000):
    pass


def main():
  led = LED()
  sw = SW()

  while True:
    count = sw.sense()
    led.out_bdigit(count)

if __name__ == '__main__':
  main()
