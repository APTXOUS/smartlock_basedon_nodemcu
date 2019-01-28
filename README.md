# 基于nodemcu的智能锁
进度
* 搭建初始框架
* C/S通讯
* 包格式定义和实现
* 功能搭建
* 数据库设计与实现

客户端   
硬件设置
* nodemcu开发板
* 锁（机械部分）
* 扩展部分

预期功能
* 在无配置的情况下，进入AP模式，人工设置ssid和pwd
    * 初始名称根据网卡MAC地址计算，以nodemcu开头
    * 密码为12345678
* 配置完毕之后，连接路由器
   * 有外网访问能力
       * 访问服务器，交换数据
	* 无外网访问能力
    	* 局域网模式 	
###固件依赖
* file
* GPIO
* net
* node
* timer
* UART
* Wifi
* end user setup
* http
* SJSON