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
    init_conf ={}
    if(file.exists("init_conf.json")) then
        local sjson = require "sjson"
        print("start to get init_conf from file")
        file.open("wifi_conf.json","r")
        result=file.readline()
        print(result)
        data=sjson.decode(result)
        -- get init_file from ths wifi.json
        conf["init_file"]=data["init_file"]
        print("end to get conf from file")
        file.close("wifi.json")
    else
        conf["init_file"]="init_client.lua"
    end


    if(file.exists(conf["init_file"])) then
        dofile(conf["init_file"])
        print("end to start wifi")
    else
        print("ERROR:lost file")
        return 
    end

    
end

tmr.alarm(0, 2000, tmr.ALARM_SINGLE, init) 

