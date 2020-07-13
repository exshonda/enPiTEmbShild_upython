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
    self.led1.low()
    self.led2.low()  
    self.led3.low()  
    self.led4.low()

  def out_bdigit(self, led_data):
    if (led_data & 0x01) == 0:
      self.led1.low()
    else:
      self.led1.high()
    if (led_data & 0x02) == 0:
      self.led2.low()
    else:
      self.led2.high()
    if (led_data & 0x04) == 0:
      self.led3.low()
    else:
      self.led3.high()
    if (led_data & 0x08) == 0:
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

  def push1_int(self,handler,pri):
    self.push1.irq(handler, Pin.IRQ_RISING, pri)

  def push2_int(self,handler,pri):
    self.push2.irq(handler, Pin.IRQ_RISING, pri)

def busy_wait():
  for _ in range(100000):
    pass

def push1_isr(pin):
  global led_count
  global led
  led_count += 1
  led.out_bdigit(led_count)

def push2_isr(pin):
  global led_count
  global led
  led_count -= 1
  led.out_bdigit(led_count)

def main():
  global led_count
  global led

  machine.disable_irq()
  led = LED()
  push = PUSH()
  push.push1_int(push1_isr, 1)
  push.push2_int(push2_isr, 1)
  led_count = 0
  machine.enable_irq()

  while True:   
    pass

if __name__ == '__main__':
  main()
