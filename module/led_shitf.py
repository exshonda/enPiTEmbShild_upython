from enPiTEmbShild import LED

def busy_wait():
  for _ in range(100000):
    pass

def main():
  led = LED()
  while True:
    led.out_bdigit(LED.LED1)
    busy_wait()
    led.out_bdigit(LED.LED2)
    busy_wait()
    led.out_bdigit(LED.LED3)
    busy_wait()
    led.out_bdigit(LED.LED4)
    busy_wait()

if __name__ == '__main__':
  main()
