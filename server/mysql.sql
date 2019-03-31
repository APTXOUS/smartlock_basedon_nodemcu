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

