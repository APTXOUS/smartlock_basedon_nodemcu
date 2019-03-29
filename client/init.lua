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
        file.open("init_conf.json","r")
        result=file.readline()
        print(result)
        data=sjson.decode(result)
        -- get init_file from ths wifi.json
        init_conf["init_file"]=data["init_file"]
        init_conf["work_file"]=data["work_file"]
        print( init_conf["work_file"])
        print("end to get conf from file")
        file.close("init_conf.json")
    else
        init_conf["init_file"]="init_client.lua"
    end


    if(file.exists(init_conf["init_file"])) then
        dofile(init_conf["init_file"])
        print("end to start wifi")
    else
        print("ERROR:lost file")
        return 
    end

    tmr.alarm(1, 1000, tmr.ALARM_AUTO, function()
        if wifi.sta.getip() == nil then
            print('Waiting for IP ...')
        else
            print('IP is ' .. wifi.sta.getip())
            local config_save=wifi.sta.getconfig(true)
            print(config_save["ssid"])
            config_obj = sjson.encoder(config_save)
            config_string=config_obj:read()
            fd=file.open("wifi_conf.json","w+")
            fd:writeline(config_string)
            fd:close()
            tmr.stop(1)
            print('start to work')
            if(file.exists(init_conf["work_file"])) then
                dofile(init_conf["work_file"])
                print("end to start wifi")
            else
                print("ERROR:lost file")
                return 
            end
        end
    end)

    
end

tmr.alarm(0, 2000, tmr.ALARM_SINGLE, init) 

