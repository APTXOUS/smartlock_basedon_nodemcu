<?php


function send_udp_message($host, $port, $message)
{
    $socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
    @socket_connect($socket, $host, $port);
 
    $num = 0;
    $length = strlen($message);
    do
    {
        $buffer = substr($message, $num);
        $ret = @socket_write($socket, $buffer);
        $num += $ret;
    } while ($num < $length);
 
    socket_close($socket);
    // UDP 是一种无链接的传输层协议, 不需要也无法获取返回消息
    return true;
}

function receive_udp_message($host, $port, $callback)
{
    $socket = socket_create(AF_INET, SOCK_DGRAM, SOL_UDP);
 
    @socket_bind($socket, $host, $port);
    @set_time_limit(0);
 
    while (true)
    {
        usleep(1000);
 
        $ret = @socket_recvfrom($socket, $request, 16384, 0, $remote_host, $remote_port);
        if ($ret)
        {
            $callback($remote_host, $remote_port, $request);
        }
 
        // 不需要返回给客户端任何消息, 继续循环
    }
}


?>