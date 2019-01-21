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
    
end

tmr.alarm(0, 2000, tmr.ALARM_SINGLE, init)