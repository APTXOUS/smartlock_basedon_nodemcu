<?php
    header('Content-Type:text/html; charset=gb2312;');
    include_once("fileSystem.php");
    include_once("database.php");

    if (empty($_POST)) {
        exit("���ύ�ı����ݳ���post_max_size! <br>");
    }

    // �ж�����������ȷ�������Ƿ���ͬ
    $password = $_POST['password'];
    $confirmPassword = $_POST['confirmPassword'];
    if ($password != $confirmPassword) {
        echo "�����������ȷ�����벻��ȣ�";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/register.html';},1000);
            </script>
        ";//�������ʹ��js 1�����ת����¼ҳ������;
    }

    $uid = $_POST['uid'];
    $username = $_POST['username'];

    // �ж��û����Ƿ��ظ�
    $userNameSQL = "select count(*) from user where uid = '$uid'";
    $mysqli=getConnect();
    $result = $mysqli->query($userNameSQL);
    $row=$result->fetch_assoc();
    if ($row["count(*)"]==1) {
        echo "�û����ѱ�ʹ��,��ʹ�������û���";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/register.html';},1000);
            </script>
        ";//�������ʹ��js 1�����ת����¼ҳ������;
    }

    $result->close();

    $registerSQL = "insert into user values('$uid', '$username')";
    $result = $mysqli->query($registerSQL);
    $registerSQL = "insert into security values('$uid', md5('$password'))";
    $result = $mysqli->query($registerSQL);



    $userSQL = "select count(*) from user where uid = '$uid'";
    $result1 = $mysqli->query($userSQL);

    $row1=$result1->fetch_assoc();  
    if($row1["count(*)"]==1)
    {
        echo "ע��ɹ�";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/login.html';},1000);
            </script>
        ";
    }
    else
    {
        echo "ע�����";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/register.html';},1000);
            </script>
        ";//�������ʹ��js 1�����ת����¼ҳ������;
    }
    $mysqli->close();
?>