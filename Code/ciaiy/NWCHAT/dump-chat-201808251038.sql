-- MySQL dump 10.13  Distrib 5.7.18, for Linux (x86_64)
--
-- Host: localhost    Database: chat
-- ------------------------------------------------------
-- Server version	5.7.18-1

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `frdList`
--

DROP TABLE IF EXISTS `frdList`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `frdList` (
  `ID` int(11) NOT NULL,
  `frdID` int(11) NOT NULL,
  `status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `frdList`
--

LOCK TABLES `frdList` WRITE;
/*!40000 ALTER TABLE `frdList` DISABLE KEYS */;
INSERT INTO `frdList` VALUES (10000,10001,0),(10001,10000,0),(10008,10000,0),(10000,10008,0),(10009,10000,0),(10000,10009,0),(10010,10000,0),(10000,10010,0),(10000,10002,0),(10002,10000,0),(10012,10000,0),(10000,10012,0),(10014,10000,0),(10000,10014,0),(10015,10014,0),(10014,10015,0);
/*!40000 ALTER TABLE `frdList` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `groupID`
--

DROP TABLE IF EXISTS `groupID`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `groupID` (
  `groupID` int(11) NOT NULL,
  `name` char(32) NOT NULL,
  PRIMARY KEY (`groupID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `groupID`
--

LOCK TABLES `groupID` WRITE;
/*!40000 ALTER TABLE `groupID` DISABLE KEYS */;
INSERT INTO `groupID` VALUES (-2,'0'),(-1,'123');
/*!40000 ALTER TABLE `groupID` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `memList`
--

DROP TABLE IF EXISTS `memList`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `memList` (
  `groupID` int(11) NOT NULL,
  `frdID` int(11) NOT NULL,
  `status` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `memList`
--

LOCK TABLES `memList` WRITE;
/*!40000 ALTER TABLE `memList` DISABLE KEYS */;
INSERT INTO `memList` VALUES (-1,10002,3),(-1,10000,1),(-1,10001,1);
/*!40000 ALTER TABLE `memList` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `onlineList`
--

DROP TABLE IF EXISTS `onlineList`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `onlineList` (
  `ID` int(11) NOT NULL,
  `fd` varchar(100) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `onlineList`
--

LOCK TABLES `onlineList` WRITE;
/*!40000 ALTER TABLE `onlineList` DISABLE KEYS */;
INSERT INTO `onlineList` VALUES (10000,'6'),(10001,'7');
/*!40000 ALTER TABLE `onlineList` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `record`
--

DROP TABLE IF EXISTS `record`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `record` (
  `sendID` int(11) NOT NULL,
  `recvID` int(11) NOT NULL,
  `time` datetime NOT NULL,
  `msg` varchar(512) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `record`
--

LOCK TABLES `record` WRITE;
/*!40000 ALTER TABLE `record` DISABLE KEYS */;
/*!40000 ALTER TABLE `record` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `userID`
--

DROP TABLE IF EXISTS `userID`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `userID` (
  `ID` int(11) NOT NULL AUTO_INCREMENT,
  `passwd` char(128) NOT NULL,
  `name` char(32) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=10018 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `userID`
--

LOCK TABLES `userID` WRITE;
/*!40000 ALTER TABLE `userID` DISABLE KEYS */;
INSERT INTO `userID` VALUES (10000,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','ciaiy'),(10001,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','cyanide'),(10002,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','skypray'),(10003,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','cx'),(10004,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','ciaiy'),(10005,'$1$ciaiy$z90Cm/kSR3j/5wCqdoJUe1','test'),(10006,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','123'),(10007,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','123'),(10008,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','zhukaidi'),(10009,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','cx'),(10010,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','chenxin'),(10011,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','zhukaidi'),(10012,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','zkd'),(10013,'$1$ciaiy$.MuQlZp70v0qSPaTXXPdc0','cxzs'),(10014,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','test'),(10015,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','a'),(10016,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','123'),(10017,'$1$ciaiy$RXvev5Ba9cvncM/0HQ4.i1','123');
/*!40000 ALTER TABLE `userID` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'chat'
--
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2018-08-25 10:38:49
