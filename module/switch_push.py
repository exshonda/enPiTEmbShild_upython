from enPiTEmbShild import LED, PUSH

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
