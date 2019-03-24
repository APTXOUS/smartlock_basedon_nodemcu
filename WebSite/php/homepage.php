<?php
session_start();
include_once("database.php");
$mysqli=getConnect();
$userName=$_SESSION["uid"];
$loginSQL = "select count(*) from security where uid= '$userName'";
$result = $mysqli->query($loginSQL);
$row=$result->fetch_assoc();
if($row["count(*)"]==0)
{
    echo "用户名或密码错误";
    echo "
        <script>
                setTimeout(function(){window.location.href='../html/login.html';},1000);
        </script>
    ";//如果错误使用js 1秒后跳转到登录页面重试;
}
function OutputTitle(){
    echo  $_SESSION["uid"];
  }
  

function OutputModal($Machineid){
    echo '<div class="modal fade" id="'.$Machineid.'" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">
                    &times;
                </button>
                <h4 class="modal-title" id="myModalLabel">
                 '.$Machineid.'预约界面
                </h4>
            </div>
            <div class="modal-body">
                <form>
                预约时间：<input type="text" list="timelist"class="form-control">
                <datalist id="timelist">';
    for ($i=0; $i<=23; $i++)
    {
        echo "<option>" ;
        echo $i;
        echo ':00-';
        echo $i+1;
        echo ':00'."</option>";
    }
    echo            '</datalist>         
                </form>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">关闭
                </button>
                <button type="button" class="btn btn-primary">
                    提交更改
                </button>
            </div>
        </div><!-- /.modal-content -->
    </div><!-- /.modal -->
</div>';
}

function OutputMachine($Machname){
   echo '<div class=" col-sm-6">
  <div class="panel panel-default">
      <div class="panel-heading">
          <h3 class="panel-title">
              NodeMcu网关
          </h3>
      </div>
      <div class="panel-body">
          <div container>
              <div class="col-sm-3 col-xs-4">
                  <img src="../img/nodemcu.png" class="img-responsive " style="float:left;">
              </div>
              <h4>今日预约:</h4>
              <div class="progress progress-striped" style="height:10px">
                  <div class="progress-bar progress-bar-success" role="progressbar" aria-valuenow="60"
                      aria-valuemin="0" aria-valuemax="100" style="width: 40%;">
                      <span class="sr-only">40% 完成</span>
                  </div>
                  <div class="progress-bar progress-bar-info" role="progressbar" aria-valuenow="60"
                      aria-valuemin="0" aria-valuemax="100" style="width: 30%;">
                      <span class="sr-only">30% 完成（信息）</span>
                  </div>
                  <div class="progress-bar progress-bar-warning" role="progressbar" aria-valuenow="60"
                      aria-valuemin="0" aria-valuemax="100" style="width: 20%;">
                      <span class="sr-only">20% 完成（警告）</span>
                  </div>
              </div>
              <div class="row" style="float:left;">
                  <div class=" col-xs-4">
                      <button class="btn btn-default btn-lg btn-block" data-toggle="modal" data-target="#'.$Machname.'">
                          预约
                      </button>
                  </div>
                  <div class="col-xs-4">
                      <a href="html/login.html" type="button" class="btn btn-default btn-lg btn-block">
                          使用
                      </a>

                  </div>
                  <div class="col-xs-4">
                      <div class="btn-group">
                          <button type="button" class="btn btn-default dropdown-toggle  btn-lg btn-block"
                              data-toggle="dropdown">
                              更多 <span class="caret"></span></button>
                          <ul class="dropdown-menu " role="menu">
                              <li><a href="#">Tablet</a></li>
                              <li><a href="#">Smartphone</a></li>
                          </ul>
                      </div>
                  </div>'
                  .OutputModal($Machname).
              '</div>
          </div>
      </div>
  </div>
</div>';
}


?>



<html>

<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>NodeMcu 个人界面</title>
    <!-- Bootstrap 核心 CSS 文件 -->
    <link rel="stylesheet" href="//cdn.bootcss.com/bootstrap/3.3.5/css/bootstrap.min.css">
    <script src="https://cdn.staticfile.org/jquery/2.1.1/jquery.min.js"></script>
	<script src="https://cdn.staticfile.org/twitter-bootstrap/3.3.7/js/bootstrap.min.js"></script>
    <style>
    </style>
</head>
<!--相应式导航栏-->

<body>
    <nav class="navbar navbar-default" role="navigation">
        <div class="container-fluid">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target="#example-navbar-collapse">
                    <span class="sr-only">切换导航</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="#">Dashboard 控制台 <?php OutputTitle(); ?></a>
            </div>
            <div class="collapse navbar-collapse navbar-right" id="example-navbar-collapse">
                <ul id="myTab" class="nav navbar-nav">
                    <li class="active">
                        <a href="#home"  data-toggle="tab">
                            我的机器
                        </a>
                    </li>
                    <li><a href="#ios"data-toggle="tab">全部机器</a></li>
                    <li class="dropdown">
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">
                            更多 <b class="caret"></b>
                        </a>
                        <ul class="dropdown-menu">
                            <li><a href="#">正在使用</a></li>
                            <li class="divider"></li>
                            <li><a href="#">设置</a></li>
                            <li class="divider"></li>
                            <li><a href="../html/login.html">返回登陆界面</a></li>
                        </ul>
                    </li>
                </ul>
            </div>
        </div>
    </nav>
    <div id="myTabContent" class="tab-content">
        <div class="tab-pane fade in active" id="home">
            <div class="container-fluid">
                <?php OutputMachine("test_demo"); ?>
            </div>	

            
        </div>
        <div class="tab-pane fade" id="ios">
            <p>iOS 是一个由苹果公司开发和发布的手机操作系统。最初是于 2007 年首次发布 iPhone、iPod Touch 和 Apple 
                TV。iOS 派生自 OS X，它们共享 Darwin 基础。OS X 操作系统是用在苹果电脑上，iOS 是苹果的移动版本。</p>
        </div>
        <div class="tab-pane fade" id="jmeter">
            <p>jMeter 是一款开源的测试软件。它是 100% 纯 Java 应用程序，用于负载和性能测试。</p>
        </div>
        <div class="tab-pane fade" id="ejb">
            <p>Enterprise Java Beans（EJB）是一个创建高度可扩展性和强大企业级应用程序的开发架构，部署在兼容应用程序服务器（比如 JBOSS、Web Logic 等）的 J2EE 上。
            </p>
        </div>
    </div>


</body>

</html>