print('start to connect to server...')
cl = net.createConnection(net.TCP, 0)
cl:connect(4000, "119.28.139.244")

cl:on("receive", receive)
cl:on("disconnection", function(sck, c) print("disconnection!") end)

cl:send("Hello Sever!")


function receive(sck,c)
    print(c)
end


