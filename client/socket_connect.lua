-- ����������
-- socket 
-- ������� ����������ִ�дӻ����͹����İ� ת������������
-- ������� ���������͹����İ� ת����ִ�дӻ� ���Ǵ������ӻ�

-- ����
-- ָʾ����

--
--struct package
--{
--    char IfM;       //�ǿͻ��˻���app
--    char Id[15];    //15λ�豸ID
--    char Divide;    //���ı�ʾ
--    char Seq;       //Ϊ0��ʾ���ְ�������Ϊ0��ʾΪ�ڼ�����
--    char Len;       //������
--    char Type;      //������
--    char Info[100]; //����
--};
--
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
    elseif(command_type==13)
    then
    end
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