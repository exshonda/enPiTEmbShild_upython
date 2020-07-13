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

def timer2_handler(tim):
  global led_count
  led_count += 1
  led_out_bdigit(led_count)

def timer2_init():
  tim = pyb.Timer(4)
  tim.init(freq=1)
  tim.callback(timer2_handler)

def main():
  global led_count

  led_init()
  timer2_init()
  led_count = 0

  while True:
    pass

if __name__ == '__main__':
  main()
