
<?php
    session_start();
    header('Content-Type:text/html; charset=gb2312;');
    include_once("database.php");
    include_once("udpfunction.php");
    // $userName = $_POST['userName'];
    // $password = $_POST['password'];
   
    if($_POST['command']=='getimg')
    {
        $mysqli=getConnect();
        $loginSQL = "select * from  EdpBinData order by collect_time desc limit 1";
        $result = $mysqli->query($loginSQL);
        $arr=array();
        while($row=$result->fetch_assoc())
        {
            array_push($arr,$row);
        }

       
        echo json_encode($arr);
        $mysqli->close();
    }

    if($_POST['command']=="ifhasperson")
    {
        $mysqli=getConnect();
        $loginSQL = "select * from  OnenetMachine where On_Machine_id= 527923817 ";
        $result = $mysqli->query($loginSQL);
        while($row=$result->fetch_assoc())
        {
            $re=$row;
        }
        echo json_encode($re);
        $mysqli->close();

    }

    if($_POST['command']=="besleep")
    {
        $mysqli=getConnect();
        $loginSQL = " update OnenetMachine set Status = \"SLEEPING\" where On_Machine_id=\"527923817\"; ";
        $result = $mysqli->query($loginSQL);
        $re="success";
        echo json_encode($re);
        $mysqli->close();

        $host = '127.0.0.1';
        $port = '1234';


        $Divide=0;
        $Seq=0;
        $Len=35;
        $Type=21;
        $message="ATHISISTHEWEBAPP";
        $package=sprintf("%s%c%c%c%c%15s",$message,$Divide,$Seq,$Len,$Type,"nodemcu00010002"); 
        //echo $package;
        send_udp_message($host, $port, $package);
    }

    if($_POST['command']=="openthedoor")
    {

        $host = '127.0.0.1';
        $port = '1234';

        $uid_len=strlen($userid);

        $Divide=0;
        $Seq=0;
        $Len=35+20;
        $Type=10;//10为手机验证
        $message="ATHISISTHEWEBAPP";
        $package=sprintf("%s%c%c%c%c%20s%15s",$message,$Divide,$Seq,$Len,$Type,$userid,"nodemcu00010002"); 
        //echo $package;
        send_udp_message($host, $port, $package);
    }

    if($_POST['command']=="camerasleep")
    {

        $mysqli=getConnect();
        $loginSQL = " update OnenetMachine set Status = \"SLEEPING\" where On_Machine_id=\"527923817\"; ";
        $result = $mysqli->query($loginSQL);
        $re="success";
        echo json_encode($re);
        $mysqli->close();

    }

    if($_POST['command']=="camerawake")
    {

        $mysqli=getConnect();
        $loginSQL = " update OnenetMachine set Status = \"FINDPEOPLE\" where On_Machine_id=\"527923817\"; ";
        $result = $mysqli->query($loginSQL);
        $re="success";
        echo json_encode($re);
        $mysqli->close();

    }
    if($_POST['command']=="sendmessage")
    {
        $words=$_POST['data'];
        $words=str_replace("%",'',urlencode($words));
        $mysqli=getConnect();
        $loginSQL = " INSERT INTO WordList (Words,Status) VALUES(\"$words\",\"tobesend\"); ";
        $result = $mysqli->query($loginSQL);
        $re="success";
        echo json_encode($words);
        $mysqli->close();

    }

    if($_POST['command']=="sendvoicekey")
    {
        $words=$_POST['data'];
        $mysqli=getConnect();
        $loginSQL = " INSERT INTO KeyList (Words,Status) VALUES(\"$words[0]\",\"tobesend\"); ";
        $result = $mysqli->query($loginSQL);
        $re="success";
        echo json_encode(urlencode($words));
        $mysqli->close();

    }

    
?>
