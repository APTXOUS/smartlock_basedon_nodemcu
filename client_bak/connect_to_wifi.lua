
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

print(conf["station_ssid"])
print(conf["station_pwd"])


station_cfg={}
station_cfg.ssid=conf["station_ssid"]
station_cfg.pwd=conf["station_pwd"]
station_cfg.save=false

wifi.sta.config(station_cfg)
wifi.sta.connect()

print("now end to connect to wifi")

