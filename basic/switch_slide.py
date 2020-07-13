from machine import Pin

def led_init():
  global led1,led2,led3,led4
  led1 = Pin('PA8', Pin.OUT, Pin.PULL_NONE)
  led2 = Pin('PA7', Pin.OUT, Pin.PULL_NONE)
  led3 = Pin('PA6', Pin.OUT, Pin.PULL_NONE)
  led4 = Pin('PA5', Pin.OUT, Pin.PULL_NONE)

def switch_slide_init():
  global sw1,sw2,sw3,sw4
  sw1 = Pin('PB3', Pin.IN, Pin.PULL_NONE)
  sw2 = Pin('PB4', Pin.IN, Pin.PULL_NONE)
  sw3 = Pin('PB5', Pin.IN, Pin.PULL_NONE)
  sw4 = Pin('PB6', Pin.IN, Pin.PULL_NONE)

def busy_wait():
  for _ in range(100000):
    pass

def led_out_bdigit(led_data):
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

def slide_sense():
  reg = 0
  if sw1.value() == 1:
    reg = 0x01
  if sw2.value() == 1:
    reg |= 0x02
  if sw3.value() == 1:
    reg |= 0x04
  if sw4.value() == 1:
    reg |= 0x08
  return reg

def main():
  led_init()
  switch_slide_init()
  while True:
    count = slide_sense()
    led_out_bdigit(count)

if __name__ == '__main__':
  main()
