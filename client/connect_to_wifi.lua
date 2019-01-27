
--connect to wifi

print("now start to connect to wifi")

local conf = nil
wifi.setmode(wifi.STATIONAP)
--if the conf exits,get conf from conf
--if not,be the AP mode and get the conf 
if file.exists("wifi.json") then 
    print("the wifi.json exits")
    conf=dofile("get_conf_from_json.lua")
else
    print("the wifi.conf not exit")
    conf=dofile("get_conf_from_http.lua")
end

if(conf ~=nil) then
    wifi.sta.config(config.wifi)
    wifi.sta.autoconnect(1)
else
    conf=dofile("get_conf_from_http.lua")
end

print(conf.wifi.sta.ssid)
print(conf.wifi.sta.pwd)




