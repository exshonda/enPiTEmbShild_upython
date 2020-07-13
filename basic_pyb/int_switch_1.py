import pyb, machine

def led_init():
  global led1,led2,led3,led4
  led1 = pyb.Pin('D7', pyb.Pin.OUT_PP)  #LED1  
  led2 = pyb.Pin('D11', pyb.Pin.OUT_PP) #LED2
  led3 = pyb.Pin('D12', pyb.Pin.OUT_PP) #LED3
  led4 = pyb.Pin('D13', pyb.Pin.OUT_PP) #LED4
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

def exti15_10_irqhandler(line):
  global led_count
  led_out_bdigit(led_count)
  led_count += 1

def switch_push_init():
  global ext
  ext = pyb.ExtInt("D2", pyb.ExtInt.IRQ_RISING, pyb.Pin.PULL_DOWN, exti15_10_irqhandler)

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
