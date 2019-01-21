--connect to wifi
--I think I should use a better way to change the settings
--the firmware need end user setup module
print('Setting up WIFI...')
wifi.setmode(wifi.STATIONAP)
wifi.ap.config({ssid="NODEMCU", pwd="12345678", auth=wifi.WPA2_PSK})

print("ap ip:"..wifi.ap.getip())
print("ap mac:"..wifi.ap.getmac())
print("sta mac:"..wifi.sta.getmac())


enduser_setup.manual(true)  
enduser_setup.start(
  function()
    print("Connectedto   wifi ip is:" .. wifi.sta.getip())   
  end,
  function(err, str)
    print("enduser_setup: Err #" .. err .. ": " .. str)
  end
)

