-- ����������
-- socket 
-- ������� ����������ִ�дӻ����͹����İ� ת������������
-- ������� ���������͹����İ� ת����ִ�дӻ� ���Ǵ������ӻ�

-- ����
-- ָʾ����
IFfinger=0
IFsound=0

-- ֮��ĳ�json�ļ�
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
    --����дҪ�ɵ�����
end)


tmr.alarm(0, 3000, tmr.ALARM_AUTO, function() --ÿ3�뷢��һ������
    if(IFfinger~=0)--��IFfingerΪ�ߵ�ƽʱ
    then

    elseif(IFsound~=0)-- ��sound���ź�ʱ
    then
        
    else
        --������
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