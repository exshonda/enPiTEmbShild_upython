import pyb

def led_init():
  global led1,led2,led3,led4
  led1 = pyb.Pin('D7', pyb.Pin.OUT_PP)  #LED1  
  led2 = pyb.Pin('D11', pyb.Pin.OUT_PP) #LED2
  led3 = pyb.Pin('D12', pyb.Pin.OUT_PP) #LED3
  led4 = pyb.Pin('D13', pyb.Pin.OUT_PP) #LED4

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

def timer2_init():
  global timer2
  timer2 = pyb.Timer(2, prescaler=41, period=0x3fffffff)#1us count : 84MHZ/2/(83+1) 

def timer2_wait(wait_s):
  timer2.counter(0)
  while timer2.counter() < (1000000 * wait_s):
    pass

def main():
  count = 0

  led_init()
  timer2_init()

  while True:  
    led_out_bdigit(count)
    count += 1
    timer2_wait(1)

if __name__ == '__main__':
  main()
