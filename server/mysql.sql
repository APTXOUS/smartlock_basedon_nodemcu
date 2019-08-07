use NodeMcu;


Create table Machine_base (
	Machine_id char(15) primary key,
	Machine_Ip integer ,
	Machine_Port integer ,
	Machine_Type integer ,
	Machine_Lastack integer
	);





Create table STM32_base (
	stm_id char(15) primary key,
	stm_name varchar(1024) ,
  stm_platform varchar(1024),
  stm_page varchar(1024)
	);


Create table Servicelist (
	Machine_id char(15),
	Machine_time varchar(1024),
  Machine_used varchar(1024)
	);



Create table History (
	his_id integer primary key not null auto_increment,
	his_user char(20) ,
  his_oper varchar(1024),
  his_mach varchar(1024),
  his_time varchar(1024)
	);


Create table UserService (
  uid char(20) NOT NULL,
  Machine_id char(15),
	Machine_time varchar(1024),
  ServicePwd char(4),
  PwdNum integer
	);




CREATE TABLE `user` (
  `uid` char(20) NOT NULL,
  `username` char(20) DEFAULT '',
  PRIMARY KEY (`uid`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=gbk;

CREATE TABLE security (
  `uid` char(20) NOt NULL,
  `password_MD5` char(32) NOT NULL,
  KEY `uid` (`uid`),
  CONSTRAINT `security_ibfk_1` FOREIGN KEY (`uid`) REFERENCES `user` (`uid`)
) ENGINE=InnoDB DEFAULT CHARSET=gbk;



Create table Machine_finger (
  `MF_Machine_id` char(15) NOt NULL,
  `MF_Machine_finger` char(15) NOt NULL,
  FOREIGN KEY (`MF_Machine_id`) REFERENCES `Machine_base` (`Machine_id`),
  FOREIGN KEY (`MF_Machine_finger`) REFERENCES `Machine_base` (`Machine_id`)
	);


Create table Machine_sound (
  `MS_Machine_id` char(15) NOt NULL,
  `MS_Machine_sound` char(15) NOt NULL,
  FOREIGN KEY (`MS_Machine_id`) REFERENCES `Machine_base` (`Machine_id`),
  FOREIGN KEY (`MS_Machine_sound`) REFERENCES `Machine_base` (`Machine_id`)
	);



Create table OnenetMachine (
  `On_Machine_id` integer NOt NULL,
  `Status` varchar(20)
 	);

drop table EdpBinData;
Create table EdpBinData(
  `logid` integer primary key not null auto_increment,
  `src_devid` varchar(100),
  `desc_json_str` varchar(100),
  `save_location`  varchar(100),
  `save_binlen` integer,
  `collect_time` DATETIME
)

drop table WordList;
Create table WordList(
  `Wordid` integer primary key not null auto_increment,
  `Words` varchar(100),
  `Status`  varchar(100),
  `Des_id` varchar(100)
)

drop table KeyList;
Create table KeyList(
  `Keyid` integer primary key not null auto_increment,
  `Words` varchar(100),
  `Status`  varchar(100),
  `Des_id` varchar(100)
)






INSERT INTO STM32_base
VALUES ("stm32f400010001","家用识别门锁","onene平台","https://open.iot.10086.cn/app_editor/#/view?openid=14e3cb75a71df97266a24f9ef00786c5");

INSERT INTO Machine_finger
VALUES ("nodemcu00010002","nodemcu00020001");

INSERT INTO OnenetMachine
VALUES(527923817,"SLEEPING")
