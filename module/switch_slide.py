from enPiTEmbShild import LED, SW

def busy_wait():
  for _ in range(100000):
    pass

def main():
  led = LED()
  sw = SW()

  while True:
    count = sw.sense()
    led.out_bdigit(count)

if __name__ == '__main__':
  main()
