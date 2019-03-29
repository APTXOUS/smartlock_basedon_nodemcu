
<?php
    session_start();
    header('Content-Type:text/html; charset=gb2312;');
    include_once("database.php");
    include_once("udpfunction.php");
    $userid=$_SESSION["uid"];
    echo  $_SESSION["uid"];
    echo $userid;
    $identity= addslashes($_POST['identity']);
    $identity=iconv("UTF-8","gbk//TRANSLIT",$identity);
    echo $identity;
    $machineid= addslashes($_POST['machineid']);
    $machineid=iconv("UTF-8","gbk//TRANSLIT",$machineid);
    echo $machineid;
    


    if($identity=="手机验证")
    {
        //$loginSQL = "select count(*) from security where uid= '$userName' AND password_MD5= md5('$password')";
        //$result = $mysqli->query($loginSQL);
        //$row=$result->fetch_assoc();
        $host = '127.0.0.1';
        $port = '1234';

        $uid_len=strlen($userid);

        $Divide=0;
        $Seq=0;
        $Len=35+20;
        $Type=10;//10为手机验证
        $message="ATHISISTHEWEBAPP";
        $package=sprintf("%s%c%c%c%c%20s15s",$message,$Divide,$Seq,$Len,$Type,$userid,$machineid); 
        echo $package;
        send_udp_message($host, $port, $package);
    }
    else if($identity=="口令验证")
    {
        header("refresh:0;url=../php/homepage.php");//如果成功跳转至homepage.html页面
        exit;
    }else if($identity=="指纹验证")
    {
        header("refresh:0;url=../php/homepage.php");//如果成功跳转至homepage.html页面
        exit;
    }
?>
