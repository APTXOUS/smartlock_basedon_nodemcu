<?php
    include_once("function/database.php");
    // $userName = $_POST['userName'];
    // $password = $_POST['password'];
    $userName = addslashes($_POST['userName']);
    $password = addslashes($_POST['password']);
    getConnect();
    $loginSQL = "select count(*) from users where userName='$userName' and password='$password'";
    echo $loginSQL; 
    $resultLogin = mysql_query($loginSQL);
    if (mysql_num_rows($resultLogin) > 0) {
        echo "µÇÂ¼³É¹¦";
    } else {
        echo "µÇÂ¼Ê§°Ü";
    }
    closeConnect();
?>
