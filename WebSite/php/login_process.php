
<?php
    session_start();
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
        $_SESSION["uid"] = $userName;
        header("refresh:0;url=../php/homepage.php");//如果成功跳转至homepage.html页面
        exit;
    }
    else
    {
        echo "用户名或密码错误";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/login.html';},1000);
            </script>
        ";//如果错误使用js 1秒后跳转到登录页面重试;
    }
    $mysqli->close();
?>
