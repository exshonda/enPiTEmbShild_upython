from enPiTEmbShild import LED, PUSH

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
