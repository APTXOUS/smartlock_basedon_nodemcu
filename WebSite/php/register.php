<?php
    include_once("function/fileSystem.php");
    include_once("function/database.php");

    if (empty($_POST)) {
        exit("您提交的表单数据超过post_max_size! <br>");
    }

    // 判断输入密码与确认密码是否相同
    $password = $_POST['password'];
    $confirmPassword = $_POST['confirmPassword'];
    if ($password != $confirmPassword) {
        exit("输入的密码与确认密码不相等！");
    }

    $userName = $_POST['uid'];
    $domain = $_POST['domain'];
    $userName = $userName . $domain;

    // 判断用户名是否重复
    $userNameSQL = "select * from user where uid = '$userName'";
    echo $userNameSQL ;
    getConnect();
    $resultSet = mysql_query($userNameSQL);
    echo $resultSet;
    if (mysql_num_rows($resultSet) > 0) {
        exit("用户名已被占用，请更换其他用户名");
    }

    $sex = $_POST['sex'];
    if (empty($_POST['interests'])) {
        $interests = "";
    } else {
        $interests = implode(";", $_POST['interests']);
    }

    $remark = $_POST['remark'];
    $myPictureName = $_FILES['myPicture']['name'];

    $registerSQL = "insert into users values(null, '$userName', '$password', '$sex', '$interests', '$myPictureName', '$remark')";
    $message = upload($_FILES['myPicture'], "uploads");

    if ($message == "上传成功" || $message == "没有上传") {
        mysql_query($registerSQL);
        $userID = mysql_insert_id();
        echo "注册成功<br>";
    } else {
        exit($message);
    }

    $userSQL = "select * from users where user_id = '$userID'";
    $userResult = mysql_query($userSQL);
    if ($user = mysql_fetch_array($userResult)) {
        echo "您的注册用户名为：" . $user['userName'];
    } else {
        exit("用户注册失败！");
    }
    closeConnect();
?>