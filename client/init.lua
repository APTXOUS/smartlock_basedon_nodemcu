print('Setting up WIFI...')
wifi.setmode(wifi.STATION)
wifi.sta.config({ssid="APTXWIFI",pwd="12345678"})
wifi.sta.connect()

function reconnect_wifi()
    if wifi.sta.getip() == nil then
        print("Waiting for IP ...")
    else
        print("IP is " .. wifi.sta.getip())
        tmr.stop(1) 
        func=dofile("main.lua")
    end
end

tmr.alarm(1, 1000, tmr.ALARM_AUTO, reconnect_wifi)

    