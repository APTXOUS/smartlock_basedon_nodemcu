--get conf from file


print("start to get conf from file")


local conf = {}

file.open("wifi.conf","r")
result=file.readline()
while(result~=nil)
do
    print(result)
    result=file.readline()
-- if(result=="[WIFIMODE]") then
--     result=file.readline()
--     if(result=="STATION") then
--         conf.wifi.mode=wifi.STATION
--     elseif(result=="SOFTAP") then
--         conf.wifi.mode=wifi.SOFTAP
--     end
-- elseif (result=="[SSID]") then
--     result=file.readline()
--     conf.wifi.sta.ssid=result;
-- elseif (result=="[PWD]") then
--     result=file.readline()
--     conf.wifi.sta.pwd=result;
-- end

end
file.close()

print("end to get conf from file")

return conf