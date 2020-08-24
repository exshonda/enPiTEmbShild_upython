import utime

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

def time_oh(f, n):
	t0 = utime.ticks_us()
	f(n)
	t1 = utime.ticks_us()
	dt = utime.ticks_diff(t1, t0)
	fmt ='utime.ticks_us() oh : {:f} usec'
	print(fmt.format(dt))

N = 30000

# F401RE : 9.50 sec
time_oh(Fib, N)
