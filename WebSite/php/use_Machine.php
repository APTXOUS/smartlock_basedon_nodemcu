
<?php

    session_start();
    header('Content-Type:text/html; charset=gb2312;');
    include_once("database.php");
    include_once("udpfunction.php");
    $userid=$_SESSION["uid"];
    //echo $userid;
    $identity= addslashes($_POST['identity']);
    $identity=iconv("UTF-8","gbk//TRANSLIT",$identity);
    //echo $identity;
    $machineid= addslashes($_POST['machineid']);
    $machineid=iconv("UTF-8","gbk//TRANSLIT",$machineid);
    //echo $machineid;
    
    $mysqli=getConnect();

   


        //$result = $mysqli->query($loginSQL);
        //$row=$result->fetch_assoc();

    if(check_var(($_POST['timerange'])))
    {

        $time_range=$_POST['timerange'];
        $id=$_POST['machineid'];
        $loginSQL = "INSERT INTO History (his_user,his_oper,his_mach,his_time)VALUES ( '$userid','ԤԼ','$id',now());";
        $result = $mysqli->query($loginSQL);

        $machineselect="select count(*) from Servicelist where Machine_id= '$id' and Machine_time ='$time_range' and Machine_used ='Not used'";
        $result = $mysqli->query($machineselect);
        $row=$result->fetch_assoc();
        if($row["count(*)"]==0)
        {
            echo "��ʱ���ѱ�ԤԼ";
            echo "
            <script>
                    setTimeout(function(){window.location.href='../php/homepage.php';},1000);
            </script>
        ";//�������ʹ��js 1�����ת����¼ҳ������;
        }
        else
        {
            $SQL="UPDATE Servicelist SET Machine_used = 'Using' WHERE Machine_id = '$id' and Machine_time ='$time_range'";
            $result = $mysqli->query($SQL);

            $pwd= mt_rand(999, 9999);

            $SQL="INSERT INTO UserService VALUES ('$userid','$machineid','$time_range','$pwd',0);";
            $result = $mysqli->query($SQL);
            echo "����������:".$pwd;
            echo "
            <script>
                    setTimeout(function(){window.location.href='../php/homepage.php';},3000);
            </script>
        ";
        }
    }
    else
    {
        $i = (date( "H")+8)%24;
        $j= $i+1;
        $time_range="$i:00-$j:00";
        $id=$_POST['machineid'];
        $SQL="select count(*) from Servicelist where Machine_id= '$id' and Machine_time ='$time_range' and Machine_used ='Not used'";
        //echo $SQL;
        $result = $mysqli->query($SQL);
        $row=$result->fetch_assoc();
        if($row["count(*)"]==0)//�ѱ�ԤԼ
        {
            $pwd=$_POST['servicepwd'];
            $SQL="select count(*) from UserService where uid='$userid' and Machine_id= '$id' and ServicePwd= '$pwd'";
            //echo $SQL;
            $result = $mysqli->query($SQL);
            $row=$result->fetch_assoc();
            if($row["count(*)"]==0)//�ѱ�ԤԼ
            {

                $SQL="UPDATE UserService SET PwdNum= (PwdNum + 1)  where uid='$userid' and Machine_id= '$id'";
                $result = $mysqli->query($SQL);
                $SQL="select PwdNum from UserService where uid='$userid' and Machine_id= '$id'";
                //echo $SQL;
                $result = $mysqli->query($SQL);
                $row=$result->fetch_assoc();

                if($row["PwdNum"]<5)
                    echo "��Կ���󣬴���".$row["PwdNum"]."��";
                else
                {
                    $pwd= mt_rand(999, 9999);
                    $SQL="UPDATE UserService SET PwdNum=0,ServicePwd= '$pwd' where uid='$userid' and Machine_id= '$id';";
                    //echo $SQL;
                    $result = $mysqli->query($SQL);
                    echo "���������Σ�����������:".$pwd;
                }
                echo "
                <script>
                        setTimeout(function(){window.location.href='../php/homepage.php';},10000);
                </script>
                ";
                exit;
            }    
        }
       

            if($identity=="�ֻ���֤")
            {
                //$loginSQL = "select count(*) from security where uid= '$userName' AND password_MD5= md5('$password')";
                //$result = $mysqli->query($loginSQL);
                //$row=$result->fetch_assoc();

                $loginSQL = "INSERT INTO History (his_user,his_oper,his_mach,his_time)VALUES ( '$userid','$identity',' $machineid',now());";
                $result = $mysqli->query($loginSQL);


                $host = '127.0.0.1';
                $port = '1234';

                $uid_len=strlen($userid);

                $Divide=0;
                $Seq=0;
                $Len=35+20;
                $Type=10;//10Ϊ�ֻ���֤
                $message="ATHISISTHEWEBAPP";
                $package=sprintf("%s%c%c%c%c%20s%15s",$message,$Divide,$Seq,$Len,$Type,$userid,$machineid); 
                //echo $package;
                send_udp_message($host, $port, $package);
                header("refresh:0;url=../php/homepage.php");//����ɹ���ת��homepage.htmlҳ��

            
            }
            else if($identity=="������֤")
            {
                $loginSQL = "INSERT INTO History (his_user,his_oper,his_mach,his_time)VALUES ( '$userid','$identity',' $machineid',now());";
                $result = $mysqli->query($loginSQL);

                $host = '127.0.0.1';
                $port = '1234';

                $uid_len=strlen($userid);

                $Divide=0;
                $Seq=0;
                $Len=35+20;
                $Type=15;//13Ϊָ����֤
                $message="ATHISISTHEWEBAPP";
                $package=sprintf("%s%c%c%c%c%20s%15s",$message,$Divide,$Seq,$Len,$Type,$userid,$machineid); 
                send_udp_message($host, $port, $package);
                echo "��˵����������";
                echo "
                    <script>
                            setTimeout(function(){window.location.href='../php/homepage.php';},5000);
                    </script>
                ";//�������ʹ��js 1�����ת����¼ҳ������;
                
                exit;
            }else if($identity=="ָ����֤")
            {

                $loginSQL = "INSERT INTO History (his_user,his_oper,his_mach,his_time)VALUES ( '$userid','$identity',' $machineid',now());";
                $result = $mysqli->query($loginSQL);

                $host = '127.0.0.1';
                $port = '1234';

                $uid_len=strlen($userid);

                $Divide=0;
                $Seq=0;
                $Len=35+20;
                $Type=13;//13Ϊָ����֤
                $message="ATHISISTHEWEBAPP";
                $package=sprintf("%s%c%c%c%c%20s%15s",$message,$Divide,$Seq,$Len,$Type,$userid,$machineid); 
                //echo $package;
                send_udp_message($host, $port, $package);
                header("refresh:0;url=../php/homepage.php");//����ɹ���ת��homepage.htmlҳ��
                exit;
            }
            $SQL="UPDATE Servicelist SET Machine_used ='Not used' where Machine_id= '$id' and Machine_time ='$time_range'";
            //echo $SQL;
            $result = $mysqli->query($SQL);


            $SQL="delete from UserService where uid='$userid' and Machine_id= '$id' and ServicePwd= '$pwd'";
            //echo $SQL;
            $result = $mysqli->query($SQL);
            $row=$result->fetch_assoc();
        
    }
?>

