<?php
    include_once("function/fileSystem.php");
    include_once("function/database.php");

    if (empty($_POST)) {
        exit("���ύ�ı����ݳ���post_max_size! <br>");
    }

    // �ж�����������ȷ�������Ƿ���ͬ
    $password = $_POST['password'];
    $confirmPassword = $_POST['confirmPassword'];
    if ($password != $confirmPassword) {
        exit("�����������ȷ�����벻��ȣ�");
    }

    $userName = $_POST['uid'];
    $domain = $_POST['domain'];
    $userName = $userName . $domain;

    // �ж��û����Ƿ��ظ�
    $userNameSQL = "select * from user where uid = '$userName'";
    echo $userNameSQL ;
    getConnect();
    $resultSet = mysql_query($userNameSQL);
    echo $resultSet;
    if (mysql_num_rows($resultSet) > 0) {
        exit("�û����ѱ�ռ�ã�����������û���");
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

    if ($message == "�ϴ��ɹ�" || $message == "û���ϴ�") {
        mysql_query($registerSQL);
        $userID = mysql_insert_id();
        echo "ע��ɹ�<br>";
    } else {
        exit($message);
    }

    $userSQL = "select * from users where user_id = '$userID'";
    $userResult = mysql_query($userSQL);
    if ($user = mysql_fetch_array($userResult)) {
        echo "����ע���û���Ϊ��" . $user['userName'];
    } else {
        exit("�û�ע��ʧ�ܣ�");
    }
    closeConnect();
?>