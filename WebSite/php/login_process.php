
<?php
    header('Content-Type:text/html; charset=gb2312;');
    include_once("database.php");
    // $userName = $_POST['userName'];
    // $password = $_POST['password'];
    $userName = addslashes($_POST['userName']);
    $password = addslashes($_POST['password']);

    $mysqli=getConnect();
    $loginSQL = "select count(*) from security where uid= '$userName' AND password_MD5= md5('$password')";
    $result = $mysqli->query($loginSQL);
    $row=$result->fetch_assoc();
    if($row["count(*)"]==1)
    {
        header("refresh:0;url=../html/welcome.html");//����ɹ���ת��welcome.htmlҳ��
        exit;
    }
    else
    {
        echo "�û������������";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/login.html';},1000);
            </script>
        ";//�������ʹ��js 1�����ת����¼ҳ������;
    }
    $mysqli->close();
?>
