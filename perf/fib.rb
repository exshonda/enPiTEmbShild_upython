def fib(n)
  if n == 1
    return a
  elsif n == 2
    return b
  else
    a, b = 0, 1
    n -= 2
    n.times do |i|
      a, b = b, a + b
    end
    return b
  end
end

t0 = Time.now
t1 = Time.now
oh = t1.usec - t0.usec

t0 = Time.now
ret = fib(30)
t1 = Time.now
dt = t1.usec - t0.usec

printf("result %d : time %d : oh %d\n",ret,dt, oh)

t0 = Time.now
t1 = Time.now
oh = t1.usec - t0.usec

t0 = Time.now
ret = fib(30)
t1 = Time.now
dt = t1.usec - t0.usec

printf("result %d : time %d : oh %d\n",ret,dt, oh)
