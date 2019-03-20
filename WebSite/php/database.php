<?php
    function getConnect() {
        $hosthome = 'localhost';
        $database = 'NodeMcu';
        $userName = 'root';
        $password = 'yyy19980425';
        $mysqli = new mysqli($hosthome, $userName, $password,$database);
        if (mysqli_connect_errno()){
            echo "连接失败，原因为：".mysqli_connect_error();
            exit();
            }
        $result = $mysqli->query ("set names gbk");
        return  $mysqli;
    }

    function closeConnect() {

    }

?>
