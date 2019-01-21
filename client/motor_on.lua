time = 0
time = tmr.now()
gpio.mode(1, gpio.OUTPUT, PULLUP)
gpio.serout(1,1,{3,7},8)
print(tmr.now() - time)
time = tmr.now()
gpio.serout(1,1,{5000,5000},8, 1)
print(tmr.now() - time)
