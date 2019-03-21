<?php
    header('Content-Type:text/html; charset=gb2312;');
    include_once("fileSystem.php");
    include_once("database.php");

    if (empty($_POST)) {
        exit("您提交的表单数据超过post_max_size! <br>");
    }

    // 判断输入密码与确认密码是否相同
    $password = $_POST['password'];
    $confirmPassword = $_POST['confirmPassword'];
    if ($password != $confirmPassword) {
        echo "输入的密码与确认密码不相等！";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/register.html';},1000);
            </script>
        ";//如果错误使用js 1秒后跳转到登录页面重试;
    }

    $uid = $_POST['uid'];
    $username = $_POST['username'];

    // 判断用户名是否重复
    $userNameSQL = "select count(*) from user where uid = '$uid'";
    $mysqli=getConnect();
    $result = $mysqli->query($userNameSQL);
    $row=$result->fetch_assoc();
    if ($row["count(*)"]==1) {
        echo "用户名已被使用,请使用其他用户名";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/register.html';},1000);
            </script>
        ";//如果错误使用js 1秒后跳转到登录页面重试;
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
        echo "注册成功";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/login.html';},1000);
            </script>
        ";
    }
    else
    {
        echo "注册错误";
        echo "
            <script>
                    setTimeout(function(){window.location.href='../html/register.html';},1000);
            </script>
        ";//如果错误使用js 1秒后跳转到登录页面重试;
    }
    $mysqli->close();
?>