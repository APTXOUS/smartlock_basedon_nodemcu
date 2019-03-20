<?php
    function upload($file, $filePath) {
        $error = $file['error'];
        switch ($error) {
            case 0:
                $fileName = $file['name'];
                $fileTemp = $file['tmp_name'];
                $destination = $filePath . "/" . $fileName;
                move_uploaded_file($fileTemp, $destination);
                return "上传成功";
            case 1:
                return "上传超过upload_max_filesize";
            case 2:
                return "上传文件超过form的MAX_FILE_SIZE";
            case 3:
                return "附件部分上传";
            case 4:
                return "没有上传";
        }
    }
?>
