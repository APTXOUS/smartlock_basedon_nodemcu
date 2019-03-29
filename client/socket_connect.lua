-- 假设是主机
-- socket 
-- 负责接收 传感器或是执行从机发送过来的包 转发到服务器上
-- 负责接收 服务器发送过来的包 转发给执行从机 或是传感器从机


ServerIP = "148.70.242.63"
ServerPort = 1234


udpSocket = net.createUDPSocket()

MainInfo1="MMACHINETYPEONES"
Divide=0;

tmr.alarm(0, 30000, tmr.ALARM_AUTO, function() --每30秒发送一次数据
    udpSocket:send(ServerPort, ServerIP, "hello Server")
    print("has send")
end)
