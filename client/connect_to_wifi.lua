
--connect to wifi

print("now start to connect to wifi")

local conf = nil
--if the conf exits,get conf from conf
--if not,be the AP mode and get the conf 
if file.exists("wifi.conf") then 
    print("the wifi.conf exits")
    conf=dofile("get_conf_from_conf.lua")
else
    print("the wifi.conf not exit")
    conf=dofile("get_conf_from_http.lua")
end


--print(conf.wifi.sta.ssid)