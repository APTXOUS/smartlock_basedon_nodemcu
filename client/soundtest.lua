gpio.mode(3, gpio.OUTPUT)
print(gpio.read(5),"\n")

while(1)
do
gpio.write(3, gpio.HIGH)
print(gpio.read(3),"\n")
tmr.delay(100000)
gpio.write(3, gpio.LOW)
print(gpio.read(3),"\n")
tmr.delay(100000)
end

