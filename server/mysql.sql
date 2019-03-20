use NodeMcu;


Create table Machine_base (
	Machine_id char(15) primary key,
	Machine_Ip integer ,
	Machine_Port integer ,
	Machine_Type integer ,
	Machine_Lastack integer
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


