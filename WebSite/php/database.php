<?php
    function getConnect() {
        $hosthome = 'localhost';
        $database = 'NodeMcu';
        $userName = 'root';
        $password = '114514';
        $mysqli = new mysqli($hosthome, $userName, $password,$database);
        if (mysqli_connect_errno()){
            echo "error��".mysqli_connect_error();
            exit();
            }
        $result = $mysqli->query ("set names gbk");
        return  $mysqli;
    }

    function closeConnect() {
        ;
    }

    function strToGBK($strText)
    {
        $encode = mb_detect_encoding($strText, array('UTF-8','GB2312','GBK'));
        if($encode == "UTF-8")
        {
            return @iconv('UTF-8','GB18030',$strText);
        }
        else
        {
            return $strText;
        }
    }
    function check_var( $var, $default = ''){
        return( (isset($var) and !empty($var )) ? $var : (!empty($default) ? $default : false) );
        
    }

    function alert($msg) {
        echo "<script type='text/javascript'>alert('$msg');</script>";
    }
?>
