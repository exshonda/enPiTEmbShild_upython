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


class PUSH:
  PUSH1 = 1
  PUSH2 = 2
  PUSH_OFF = 0
  PUSH_ON  = 1

  def __init__(self):
    self.push1 = Pin('PA9', Pin.IN, Pin.PULL_NONE)
    self.push2 = Pin('PA10', Pin.IN, Pin.PULL_NONE)

  def sense(self):
    reg = 0
    if self.push1.value() == 1:
      reg = self.PUSH1
    if self.push2.value() == 1:
      reg |= self.PUSH2
    return reg

  def sense_push1(self):
    if self.push1.value() == 1:
      return self.PUSH_ON
    else:
      return self.PUSH_OFF

  def sense_push2(self):
    if self.push2.value() == 1:
      return self.PUSH_ON
    else:
      return self.PUSH_OFF

def busy_wait():
  for _ in range(100000):
    pass

def main():
  led_data = 0
  push1_pstate = PUSH.PUSH_OFF
  push2_pstate = PUSH.PUSH_OFF
  push1_state = PUSH.PUSH_OFF
  push2_state = PUSH.PUSH_OFF

  led = LED()
  push = PUSH()

  while True:
    push1_state = push.sense_push1()
    if ((push1_state == PUSH.PUSH_ON) and (push1_pstate == PUSH.PUSH_OFF)):
      led_data += 1
    push1_pstate = push1_state

    push2_state = push.sense_push2()
    if ((push2_state == PUSH.PUSH_ON) and (push2_pstate == PUSH.PUSH_OFF)):
      led_data -= 1
    push2_pstate = push2_state

    led.out_bdigit(led_data)

if __name__ == '__main__':
  main()
