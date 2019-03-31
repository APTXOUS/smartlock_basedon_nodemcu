-- 假设是主机
-- socket 
-- 负责接收 传感器或是执行从机发送过来的包 转发到服务器上
-- 负责接收 服务器发送过来的包 转发给执行从机 或是传感器从机

-- 监听
-- 指示变量

--
--struct package
--{
--    char IfM;       //是客户端还是app
--    char Id[15];    //15位设备ID
--    char Divide;    //包的标示
--    char Seq;       //为0表示不分包·，不为0表示为第几个包
--    char Len;       //包长度
--    char Type;      //包类型
--    char Info[100]; //属性
--};
--
Enablesound=0
Enablefinger=0

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


gpio.mode(5,gpio.OUTPUT)
uart.on("data",3,
  function(data)
    print(data)
    if(Enablesound==1)
    then
        if(data == "aaa")
        then
            message="aaaa"
            Divide=1
            Seq=1
            Len=24
            Type=16-- 声音确认包
            MainInfo1=string.format("N%s%c%c%c%c%s", init_conf["name"],Divide,Seq,Len,Type,message)
            udpSocket:send(ServerPort, ServerIP, MainInfo1)
            print("has send sound :",MainInfo1,"\n")
            Enablesound=0
        end
        if(data =="bbb")
        then 
            message="bbbb"
            Divide=1
            Seq=1
            Len=24
            Type=16-- 声音确认包
            MainInfo1=string.format("N%s%c%c%c%c%s", init_conf["name"],Divide,Seq,Len,Type,message)
            udpSocket:send(ServerPort, ServerIP, MainInfo1)
            print("has send sound :",MainInfo1,"\n")
            Enablesound=0
        end
        if(data =="ccc")
        then 
            message="cccc"
            Divide=1
            Seq=1
            Len=24
            Type=16-- 声音确认包
            MainInfo1=string.format("N%s%c%c%c%c%s", init_conf["name"],Divide,Seq,Len,Type,message)
            udpSocket:send(ServerPort, ServerIP, MainInfo1)
            print("has send sound :",MainInfo1,"\n")
            Enablesound=0
        end
    end
end,0)


udpSocket:on("receive", function(s, data, port, ip)
    print(string.format("received '%s' from %s:%d", data, ip, port))
    command_type=string.byte(data, 20,20);
    print(command_type);
    if(command_type==12)
    then
        gpio.mode(5, gpio.OUTPUT)
        print(gpio.read(5),"\n")
        gpio.write(5, gpio.HIGH)
        print(gpio.read(5),"\n")
        tmr.delay(100000)
        gpio.write(5, gpio.LOW)
        print(gpio.read(5),"\n")
        tmr.alarm(2, 4000, tmr.ALARM_SINGLE, 
        function()
            gpio.write(5, gpio.HIGH)
            print(gpio.read(5),"\n")
        end)
    elseif(command_type==13)--告诉nodemcu可以去监听指纹了
    then
        Enablefinger=1;
    elseif(command_type==15)--告诉nodemcu可以去监听声音了
    then
        Enablesound=1;
    end 
    --这里写要干的事情
end)

tmr.alarm(2, 2000, tmr.ALARM_AUTO, function()
    if(Enablefinger==1)
    then
        gpio.mode(5, gpio.INPUT)
        print(gpio.read(5),"\n")
        if(gpio.read(5)==1)
        then
            Divide=1
            Seq=1
            Len=20
            Type=14-- 指纹确认包
            MainInfo1=string.format("N%s%c%c%c%c", init_conf["name"],Divide,Seq,Len,Type)
            udpSocket:send(ServerPort, ServerIP, MainInfo1)
            print("has send finger :",MainInfo1,"\n")
            Enablefinger=0
        end
    end
end)

tmr.alarm(0, 3000, tmr.ALARM_AUTO, function() --每3秒发送一次数据
    --心跳包
    Divide=1
    Seq=1
    Len=20
    Type=15-- AWAKE_COMMAND
    MainInfo1=string.format("N%s%c%c%c%c", init_conf["name"],Divide,Seq,Len,Type)
    udpSocket:send(ServerPort, ServerIP, MainInfo1)
    print("has send:",MainInfo1,"\n")
end)

print("end")