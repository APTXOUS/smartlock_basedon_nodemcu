-- ����������
-- socket 
-- ������� ����������ִ�дӻ����͹����İ� ת������������
-- ������� ���������͹����İ� ת����ִ�дӻ� ���Ǵ������ӻ�


ServerIP = "148.70.242.63"
ServerPort = 1234


udpSocket = net.createUDPSocket()

MainInfo1="MMACHINETYPEONES"
Divide=0;

tmr.alarm(0, 30000, tmr.ALARM_AUTO, function() --ÿ30�뷢��һ������
    udpSocket:send(ServerPort, ServerIP, "hello Server")
    print("has send")
end)
