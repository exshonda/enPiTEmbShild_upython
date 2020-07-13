from machine import Pin

def led_init():
  global led1,led2,led3,led4
  led1 = Pin('PA8', Pin.OUT, Pin.PULL_NONE)
  led2 = Pin('PA7', Pin.OUT, Pin.PULL_NONE)
  led3 = Pin('PA6', Pin.OUT, Pin.PULL_NONE)
  led4 = Pin('PA5', Pin.OUT, Pin.PULL_NONE)
  led1.low()
  led2.low()  
  led3.low()  
  led4.low()

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

def busy_wait():
  for _ in range(100000):
    pass

def exti15_10_irqhandler(pin):
  global led_count
  led_count += 1
  led_out_bdigit(led_count)

def exti9_5_irqhandler(pin):
  global led_count
  led_count -= 1
  led_out_bdigit(led_count)

def switch_push_init():
  global push1,push2
  push2 = Pin('PA10', Pin.IN, Pin.PULL_UP)
  push2.irq(exti15_10_irqhandler, Pin.IRQ_RISING, 1)
  push1 = Pin('PA9', Pin.IN, Pin.PULL_UP)
  push1.irq(exti9_5_irqhandler, Pin.IRQ_RISING, 2)

def main():
  global led_count

  machine.disable_irq()
  led_init()
  switch_push_init()
  led_count = 0
  machine.enable_irq()

  while True:   
    pass

if __name__ == '__main__':
  main()
