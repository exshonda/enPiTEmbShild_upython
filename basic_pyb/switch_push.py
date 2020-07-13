import pyb

PUSH1 = 1
PUSH2 = 2

PUSH_OFF = 0
PUSH_ON  = 1

def led_init():
  global led1,led2,led3,led4
  led1 = pyb.Pin('D7', pyb.Pin.OUT_PP)  #LED1  
  led2 = pyb.Pin('D11', pyb.Pin.OUT_PP) #LED2
  led3 = pyb.Pin('D12', pyb.Pin.OUT_PP) #LED3
  led4 = pyb.Pin('D13', pyb.Pin.OUT_PP) #LED4

def switch_push_init():
  global push1,push2
  push1 = pyb.Pin('D8', pyb.Pin.IN)#PA_9
  push2 = pyb.Pin('D2', pyb.Pin.IN)#PA_10

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

def switch_push_sense():
  reg = 0
  if push1.value() == 1:
    reg = PUSH1
  if push2.value() == 1:
    reg |= PUSH2
  return reg

def main():
  led_data = 0
  push_data = 0
  push1_state = PUSH_OFF
  push2_state = PUSH_OFF

  led_init()
  switch_push_init()

  while True:
    push_data = switch_push_sense()

    if (((push_data & PUSH1) == PUSH1) and (push1_state == PUSH_OFF)):
      led_data += 1
    push1_state = PUSH_ON if ((push_data & PUSH1) == PUSH1) else PUSH_OFF

    if ((push_data & PUSH2) == PUSH2) and (push2_state == PUSH_OFF):
      led_data -= 1
    push2_state = PUSH_ON if ((push_data & PUSH2) == PUSH2) else PUSH_OFF

    led_out_bdigit(led_data)

if __name__ == '__main__':
  main()
