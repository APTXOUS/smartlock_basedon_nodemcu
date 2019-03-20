
<?php
    header('Content-Type:text/html; charset=gb2312;');
    include_once("database.php");
    // $userName = $_POST['userName'];
    // $password = $_POST['password'];
    $userName = addslashes($_POST['userName']);
    $password = addslashes($_POST['password']);

    //$mysqli=getConnect();
    $loginSQL = "select count(*) from security where uid= '$userName' AND password='$password'";
    echo $loginSQL; 
    //$result = $mysqli->query($loginSQL);
    //echo $result->num_rows;
    //closeConnect();
?>
