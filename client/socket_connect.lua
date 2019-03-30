-- 假设是主机
-- socket 
-- 负责接收 传感器或是执行从机发送过来的包 转发到服务器上
-- 负责接收 服务器发送过来的包 转发给执行从机 或是传感器从机

-- 监听
-- 指示变量
IFfinger=0
IFsound=0

-- 之后改成json文件
ServerIP = "148.70.242.63"
ServerPort = 1234


local sjson = require "sjson"
file.open("init_conf.json","r")
result=file.readline()
print(result)
data=sjson.decode(result)
init_conf["name"]=data["name"]
file.close("init_conf.json")


udpSocket = net.createUDPSocket()
udpSocket:listen(34835)

port, ip =udpSocket:getaddr()
print(port,ip)


Divide=0;

udpSocket:on("receive", function(s, data, port, ip)
    print(string.format("received '%s' from %s:%d", data, ip, port))
    --这里写要干的事情
end)


tmr.alarm(0, 3000, tmr.ALARM_AUTO, function() --每3秒发送一次数据
    if(IFfinger~=0)--当IFfinger为高电平时
    then

    elseif(IFsound~=0)-- 当sound有信号时
    then
        
    else
        --心跳包
        Divide=1
        Seq=1
        Len=20
        Type=15-- AWAKE_COMMAND
        MainInfo1=string.format("N%s%c%c%c%c", init_conf["name"],Divide,Seq,Len,Type)
        udpSocket:send(ServerPort, ServerIP, MainInfo1)
        print("has send:",MainInfo1,"\n")
    end
end)

print("end")