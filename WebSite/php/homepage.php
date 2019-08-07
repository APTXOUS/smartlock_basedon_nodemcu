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
    echo '<div class="modal fade" id="'.$Machineid."Modal".'" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
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
            <form action="../php/use_Machine.php" method="POST">
            <div class="modal-body">
                机器:
                <input type="text" name="machineid" class="form-control" value="'.$Machineid.'" readonly="readonly">
                预约时间：<input type="text"  name="timerange" list="timelist"class="form-control">
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
              
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">关闭
                </button>
                <button type="submit" class="btn btn-primary">
                    提交更改
                </button>
            </div>
            </form>
        </div><!-- /.modal-content -->
    </div><!-- /.modal -->
</div>';
}

function OutputMachine($Machname){
   echo '<div class=" col-sm-6">
  <div class="panel panel-default">
      <div class="panel-heading">
          <h3 class="panel-title">
              '.$Machname.'
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
                    <button class="btn btn-default btn-lg btn-block" data-toggle="modal" data-target="#'.$Machname."Modal".'">
                        预约
                    </button>
                  </div>
                  <div class="col-xs-4">
                    <button class="btn btn-default btn-lg btn-block" data-toggle="modal" data-target="#'.$Machname."Use".'">
                        使用
                    </button>

                  </div>
                  <div class="col-xs-4">
                      <div class="btn-group">
                          <button type="button" class="btn btn-default dropdown-toggle  btn-lg btn-block"
                              data-toggle="dropdown">
                              更多 <span class="caret"></span></button>
                          <ul class="dropdown-menu " role="menu">
                              <li><a href="#"data-toggle="modal" data-target="#'.$Machname."His".'">预约记录</a></li>
                              <li><a href="#">more</a></li>
                          </ul>
                      </div>
                  </div>'
                  .OutputModal($Machname)
                  .OutputUse($Machname)
                  .OutputHis($Machname).
              '</div>
          </div>
      </div>
  </div>
</div>';
}

function OutputHis($Machineid)
{
    
    $mysqli=getConnect();
    $loginSQL = "select count(*) from Servicelist where Machine_id= '$Machineid'";
    $result = $mysqli->query($loginSQL);
    $row=$result->fetch_assoc();
    if($row["count(*)"]==0)
    {
        for ($i=0; $i<=23; $i++)
        {
            $j= $i+1;
            $SQL="INSERT INTO Servicelist VALUES ('$Machineid','$i:00-$j:00','Not used')";
            $result = $mysqli->query($SQL);
        }
    }

    echo '<div class="modal fade" id="'.$Machineid."His".'" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">
                    &times;
                </button>
                <h4 class="modal-title" id="myModalLabel">
                 '.$Machineid.'预约记录
                </h4>
            </div>
            <form action="../php/use_Machine.php" method="POST">
                <div class="modal-body"> 
                    机器:
                    <input type="text" name="machineid" class="form-control" value="'.$Machineid.'" readonly="readonly">
                    预约记录';

    echo  '<table class="table table-striped">';
    echo '<thead>
    <tr>
      <th>时间</th>
      <th>状态</th>
    </tr>
    </thead>';

    echo '<tbody>';

    $machineselect="select * from Servicelist where Machine_id ='$Machineid'";
    $result = $mysqli->query($machineselect);
    while($row=$result->fetch_assoc()){
        echo  ' <tr>
        <td>'.$row['Machine_time'].'</td>
        <td>'.$row['Machine_used'].'</td>
      </tr>';
    }
    echo '</tbody>';
    echo  '</table>';


    echo'        </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">关闭
                    </button>
                </div>
            </form>
        </div><!-- /.modal-content -->
    </div><!-- /.modal -->
</div>';
}

function OutputUse($Machineid)
{
    echo '<div class="modal fade" id="'.$Machineid."Use".'" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">
                    &times;
                </button>
                <h4 class="modal-title" id="myModalLabel">
                 '.$Machineid.'使用界面
                </h4>
            </div>
            <form action="../php/use_Machine.php" method="POST">
                <div class="modal-body"> 
                    机器:
                    <input type="text" name="machineid" class="form-control" value="'.$Machineid.'" readonly="readonly">
                    预约密钥:
                    <input type="text" name="servicepwd" class="form-control" placeholder="请填写预约密钥，若没有密钥，则留空">
                    选择你要使用的验证方式:
                    <input type="text" name="identity" list="securitylist"class="form-control">
                    <datalist id="securitylist">
                    <option>手机验证</option>
                    <option>口令验证</option>
                    <option>指纹验证</option>
                    </datalist>         
                </div>
                <div class="modal-footer">
                    <button type="button" class="btn btn-default" data-dismiss="modal">关闭
                    </button>
                    <button type="submit" class="btn btn-primary">
                        开始验证
                    </button>
                </div>
            </form>
        </div><!-- /.modal-content -->
    </div><!-- /.modal -->
</div>';
}
function OutputAllMachine()
{
    $mysqli=getConnect();
    $machineselect="select * from Machine_base";
    $result = $mysqli->query($machineselect);
    while($row=$result->fetch_assoc()){
        OutputMachine(iconv('GB2312', 'UTF-8', $row["Machine_id"]));
    }
    $result->close();
    $mysqli->close();
}

function OutputStm($row)
{
    echo '<div class=" col-sm-6">
    <div class="panel panel-default">
        <div class="panel-heading">
            <h3 class="panel-title">
                '.$row["stm_name"].'
            </h3>
        </div>
        <div class="panel-body">
          <div container>
              <div class="col-sm-3 col-xs-3">
                  <img src="../img/nodemcu.png" class="img-responsive " style="float:left;">
              </div>
              <div class="col-sm-3 col-xs-3">
              <a href="'.$row["stm_page"].'"><input class="btn btn-primary" type="button" name="test" value="使用"/></a>
              </div>
          </div>
      </div>
    </div> 
    </div>';
}

function OutputStm32()
{
    $mysqli=getConnect();
    $machineselect="select * from STM32_base";
    $result = $mysqli->query($machineselect);
    while($row=$result->fetch_assoc()){
        OutputStm($row);
    }
    $result->close();
    $mysqli->close();
}


function OutputMyservice()
{
    echo '<div class=" col-sm-12">';
    echo '<div class="panel panel-default">
    <div class="panel-heading">
    <h3 class="panel-title">
        预约记录
    </h3>
    </div>';
    echo  '<table class="table table-striped">';
    echo '<thead>
    <tr>
      <th>机器</th>
      <th>时段</th>
      <th>密码</th>
    </tr>
    </thead>';

    echo '<tbody>';
    $userName=$_SESSION["uid"];
    $mysqli=getConnect();
    $machineselect="select * from UserService where uid='$userName'";
    $result = $mysqli->query($machineselect);
    while($row=$result->fetch_assoc()){
        echo  ' <tr>
        <td>'.$row['Machine_id'].'</td>
        <td>'.$row['Machine_time'].'</td>
        <td>'.$row['ServicePwd'].'</td>
       </button>'.'</td>
      </tr>';
    }
    echo '</tbody>';
    echo  '</table></div></div>';

    $result->close();
    $mysqli->close();
}


function OutputHistory()
{
    echo '<div class=" col-sm-12">';
    echo '<div class="panel panel-default">
    <div class="panel-heading">
    <h3 class="panel-title">
        历史记录
    </h3>
    </div>';
    echo  '<table class="table table-striped">';
    echo '<thead>
    <tr>
      <th>用户</th>
      <th>操作</th>
      <th>机器</th>
      <th>时间</th>
    </tr>
    </thead>';

    echo '<tbody>';

    $mysqli=getConnect();
    $machineselect="select * from History ORDER BY his_time DESC";
    $result = $mysqli->query($machineselect);
    while($row=$result->fetch_assoc()){
        echo  ' <tr>
        <td>'.$row['his_user'].'</td>
        <td>'.iconv('GB2312', 'UTF-8',$row['his_oper']).'</td>
        <td>'.$row['his_mach'].'</td>
        <td>'.$row['his_time'].'</td>
      </tr>';
    }
    echo '</tbody>';
    echo  '</table></div></div>';

    $result->close();
    $mysqli->close();
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
                            NodeMcu设备
                        </a>
                    </li>
                    <li><a href="#ios"data-toggle="tab">onenet设备</a></li>
                    <li><a href="#history"data-toggle="tab">使用记录</a></li>
                    <li><a href="#service"data-toggle="tab">我的预约</a></li>
                    <li class="dropdown">
                        <a href="#" class="dropdown-toggle" data-toggle="dropdown">
                            更多 <b class="caret"></b>
                        </a>
                        <ul class="dropdown-menu">
                            <li><a href="#">帮助</a></li>
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
                <?php OutputAllMachine(); ?>
            </div>	

            
        </div>
        <div class="tab-pane fade" id="ios">
            <div class="container-fluid">
                <?php OutputStm32(); ?>
            </div>	
        </div>
        <div class="tab-pane fade" id="history">
            <div class="container-fluid">
                <?php OutputHistory(); ?>
            </div>	
        </div>
        <div class="tab-pane fade" id="service">
            <div class="container-fluid">
                <?php OutputMyservice(); ?>
            </div>	
        </div>
    </div>



</div>


</body>



<script type="text/javascript">
   function auto_tab_div(){
    var start = new Date().getTime()
    $.ajax({
		type: "POST",
		url: "../php/getimg.php",
		data: {
			command: "ifhasperson"// 注意不要在此行增加逗号
		},
		dataType: "json",
		success: function (msg) {

            if(msg.Status=="FINDPEOPLE")
            {
                clearInterval(id);
                alert("有人正在您的房门前");
            }
            console.log(msg);
            
		}
    });
   }
    var id=setInterval("auto_tab_div()",2000);
</script>
</html>

</html>


