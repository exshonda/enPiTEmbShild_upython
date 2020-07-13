import pyb

def led_init():
  global led1,led2,led3,led4
  led1 = pyb.Pin('D7', pyb.Pin.OUT_PP)  #LED1  
  led2 = pyb.Pin('D11', pyb.Pin.OUT_PP) #LED2
  led3 = pyb.Pin('D12', pyb.Pin.OUT_PP) #LED3
  led4 = pyb.Pin('D13', pyb.Pin.OUT_PP) #LED4

def switch_slide_init():
  global sw1,sw2,sw3,sw4
  sw1 = pyb.Pin('D3', pyb.Pin.IN) #PB_3
  sw2 = pyb.Pin('D5', pyb.Pin.IN)#PB_4
  sw3 = pyb.Pin('D4', pyb.Pin.IN)#PB_5
  sw4 = pyb.Pin('D10', pyb.Pin.IN)#PB_6

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
