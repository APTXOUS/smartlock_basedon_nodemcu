-- init.lua
print("loading....")



print("now the client start")

l = file.list()
for name, size in pairs(l) do 
    print("name: " ..name..", size: "..size)
end


----------------------
--define
---------------------
IO_BLINK = 4

TMR_WIFI = 4
TMR_BLINK = 5
TMR_BTN = 6

gpio.mode(IO_BLINK, gpio.OUTPUT)


---------------------
-- blink
---------------------
blink = nil
tmr.register(TMR_BLINK, 100, tmr.ALARM_AUTO, function()
    gpio.write(IO_BLINK, blink.i % 2)
    tmr.interval(TMR_BLINK, blink[blink.i + 1])
    blink.i = (blink.i + 1) % #blink
end)

function blinking(param)
    if type(param) == 'table' then
        blink = param
        blink.i = 0
        tmr.interval(TMR_BLINK, 1)
        running, _ = tmr.state(TMR_BLINK)
        if running ~= true then
            tmr.start(TMR_BLINK)
        end
    else
        tmr.stop(TMR_BLINK)
        gpio.write(IO_BLINK, param or gpio.LOW)
    end
end

---------------------
-- wifi
---------------------
local sjson = require "sjson"
conf ={}
print("start to get conf from file")
if file.exists("wifi_conf.json") then 
    file.open("wifi_conf.json","r")
    result=file.readline()
    print(result)
    data=sjson.decode(result)
-- get ssid and pwd from ths wifi.json
    conf["station_ssid"]=data["ssid"]
    conf["station_pwd"]=data["pwd"]
    print("end to get conf from file")
    file.close("wifi.json")
else
    conf["station_ssid"]="WIFITEST"
    conf["station_pwd"]="12345678"
end



print('Setting up WIFI...')
wifi.setmode(wifi.STATIONAP )
wifi.sta.config({ssid=conf["station_ssid"],pwd=conf["station_pwd"]}) 
wifi.ap.config({ssid='nodemcu', pwd='12345678'}) 
wifi.sta.connect()
wifi.sta.autoconnect(1)

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
    end
end)

---------------------
-- http
---------------------
dofile('httpServer.lua')
httpServer:listen(80)

httpServer:use('/config', function(req, res)
    if req.query.ssid ~= nil and req.query.pwd ~= nil then
        wifi.sta.config({ssid=req.query.ssid, pwd=req.query.pwd})
        print(req.query.ssid)
        print(req.query.pwd)
        wifi.sta.connect()
        status = 'STA_CONNECTING'
        status_sleep=0
        tmr.alarm(TMR_WIFI, 1000, tmr.ALARM_AUTO, function()
            status_sleep=status_sleep+1
            if(status_sleep==10) then
                   res:type('application/json')
                res:send('{"status":"timeout"}')
                tmr.stop(TMR_WIFI)    
             end
            
            if status ~= 'STA_CONNECTING'  then
                res:type('application/json')
                res:send('{"status":"' .. status .. '"}')
                tmr.stop(TMR_WIFI)                
            end

            
        end)
    end
end)

-- Get json
httpServer:use('/scanap', function(req, res)

	wifi.sta.getap(function(table)
		  local aptable = {}
		  for ssid,v in pairs(table) do
			  local authmode, rssi, bssid, channel = string.match(v, "([^,]+),([^,]+),([^,]+),([^,]+)")
			  aptable[ssid] = {
				  authmode = authmode,
				  rssi = rssi,
				  bssid = bssid,
				  channel = channel
			  }
		  end
		  res:type('application/json')
		  res:send(sjson.encode(aptable)) end)
  end)
