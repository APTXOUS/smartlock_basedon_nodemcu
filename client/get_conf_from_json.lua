--get conf from file
local sjson = require "sjson"
local conf ={}

print("start to get conf from file")
file.open("wifi.json","w")
result=file.readline();
local data=sjson.decode(result)
-- get ssid and pwd from ths wifi.json
conf.wifi.sta.ssid=data["ssid"]
conf.wifi.sta.pwd=data["pwd"]

print("end to get conf from file")

return conf