import utime

def blink_empty(n):
	pass

def Fib(n):
	a, b = 0, 1
	if n == 1:
		return a
	elif n == 2:
		return b
	else:
		r = range(n-2)
		for i in r:
			a, b = b, a + b
		return b

@micropython.native
def Fib_native(n):
	a, b = 0, 1
	if n == 1:
		return a
	elif n == 2:
		return b
	else:
		r = range(n-2)
		for i in r:
			a, b = b, a + b
		return b

def time_oh(f, n):
	t0 = utime.ticks_us()
	f(n)
	t1 = utime.ticks_us()
	dt = utime.ticks_diff(t1, t0)
	fmt ='utime.ticks_us() oh : {:f} usec'
	print(fmt.format(dt))

def time_it(f, n, s = ''):
	if not s:
		s = f.__name__
	t0 = utime.ticks_us()
	fib = f(n)
	t1 = utime.ticks_us()
	dt = utime.ticks_diff(t1, t0)
	fmt ='{:10s} : {:5d} usec {:5d}'
	print(fmt.format(s, dt, fib))

N = 30

time_oh(blink_empty, N)
time_it(Fib, N)
time_it(Fib_native, N, 'Fib_native')
