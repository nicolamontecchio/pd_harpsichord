import mraa
import time

print (mraa.getVersion())
x = mraa.Gpio(13)
x.dir(mraa.DIR_OUT)

s = 0

while True:
    s = 1 if s==0 else 0
    x.write(s)
    print s
    time.sleep(1.0)
