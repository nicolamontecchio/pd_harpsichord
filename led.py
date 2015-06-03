import mraa
import time
import random

print (mraa.getVersion())


x = mraa.Gpio(13); x.dir(mraa.DIR_OUT)
print 'x'
y = mraa.Gpio(40); y.dir(mraa.DIR_OUT)
print 'y'
z = mraa.Gpio(41); z.dir(mraa.DIR_OUT)
print 'z'

w = mraa.Gpio(23); z.dir(mraa.DIR_IN)
print 'w'

while True:
    status = w.read()
    print status
    x.write(status)
    time.sleep(0.1)

# sx = 0
# sy = 0
# sz = 0

# while True:
#     sx = 1 if sx==0 else 0
#     x.write(sx)
#     print 'x', sx
#     time.sleep(random.random())
#     sy = 1 if sy==0 else 0
#     y.write(sy)
#     print 'y', sy
#     time.sleep(random.random())
#     sz = 1 if sz==0 else 0
#     z.write(sz)
#     print 'z', sz
#     time.sleep(random.random())
#     print
