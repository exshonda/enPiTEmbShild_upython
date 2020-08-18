import time, machine

led = machine.Pin('B0', machine.Pin.OUT, machine.Pin.PULL_NONE)
N = 200_000

def blink_empty(n):
	pass

def blink_simple(n):
	for i in range(n):
		led.on()
		led.off()

def blink_cache1(n):
	r = range(n)
	for i in r:
		led.on()
		led.off()

def blink_cache2(n):
	on = led.on
	off = led.off
	for i in range(n):
		on()
		off()

def blink_cache3(n):
	on = led.on
	off = led.off
	r = range(n)
	for i in r:
		on()
		off()

@micropython.native
def blink_native1(n):
	for i in range(n):
		led.on()
		led.off()

@micropython.native
def blink_native2(n):
	on = led.on
	off = led.off
	r = range(n)
	for i in r:
		on()
		off()

@micropython.viper
def blink_viper(n:int):
	p = ptr16(stm.GPIOB + stm.GPIO_BSRR)
	r = range(n)
	for i in r:
		p[1] = 1 << 4 #low
		p[0] = 1 << 4 #high

@micropython.asm_thumb
def blink_asm(r0):
	movwt(r1, stm.GPIOB + stm.GPIO_BSRR)
	mov(r2, 1 << 4)
	label(loop)
	strh(r2, [r1, 2])
	strh(r2, [r1, 0])
	sub(r0, 1)
	bne(loop)

def time_oh(f, n):
	t0 = time.ticks_us()
	f(n)
	t1 = time.ticks_us()
	dt = time.ticks_diff(t1, t0)
	fmt ='time.ticks_us() oh : {:f} usec'
	print(fmt.format(dt))

def time_it(f, n, s = ''):
	if not s:
		s = f.__name__
	t0 = time.ticks_us()
	f(n)
	t1 = time.ticks_us()
	dt = time.ticks_diff(t1, t0)
	fmt ='{:15s} : {:5.3f} sec, {:6.3f} usec/blink, {:8.2f} kblinks/sec'
	print(fmt.format(s, dt * 1e-6, dt / N, N/dt * 1e3))

time_oh(blink_empty, N)

time_it(blink_simple, N)
time_it(blink_cache1, N)
time_it(blink_cache2, N)
time_it(blink_cache3, N)
time_it(blink_native1, N, 'blink_native1')
time_it(blink_native2, N, 'blink_native2')
time_it(blink_viper,   N, 'blink_viper')
time_it(blink_asm,     N, 'blink_asm')
