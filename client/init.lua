--connect to wifi
--I think I should use a better way to change the settings
--the firmware need end user setup module

print("loading....")



function init() 
    print("now the client start")
    
    l = file.list()
    for name, size in pairs(l) do 
        print("name: " ..name..", size: "..size)
    end
    
    if(file.exists("connect_to_wifi.lua")) then
        dofile("connect_to_wifi.lua")
    else
        print("ERROR:lost file")
    end
    
    function reconnect_wifi()
        if wifi.sta.getip() == nil then
            print('Waiting for IP ...')
        else
            print('IP is ' .. wifi.sta.getip())
            tmr.stop(1) -- 连上了wifi就停止定时器1连接
        end
    end
    
    tmr.alarm(1, 1000, tmr.ALARM_AUTO, reconnect_wifi) -- 打开定时器1,1000ms重连，自动重连，处理函数
    
    if wifi.sta.getip() ~= nil then
        dofile("main_wan.lua")
    else
        dofile("main_lan.lua")
    end
end

tmr.alarm(0, 2000, tmr.ALARM_SINGLE, init)