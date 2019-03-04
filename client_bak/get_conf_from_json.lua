--get conf from file
local sjson = require "sjson"
local conf ={}

print("start to get conf from file")
file.open("wifi_conf.json","r")
local result=file.readline()
print(result)
data=sjson.decode(result)

-- get ssid and pwd from ths wifi.json

conf["station_ssid"]=data["ssid"]
conf["station_pwd"]=data["pwd"]

print("end to get conf from file")


return conf
