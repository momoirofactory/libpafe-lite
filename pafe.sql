-- phpMyAdmin SQL Dump
-- version 3.5.0
-- http://www.phpmyadmin.net
--
-- ホスト: localhost
-- 生成日時: 2013 年 6 月 26 日 22:33
-- サーバのバージョン: 5.1.69
-- PHP のバージョン: 5.3.3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- データベース: `pafe`
--

-- --------------------------------------------------------

--
-- テーブルの構造 `chime`
--

CREATE TABLE IF NOT EXISTS `chime` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `Mon` tinyint(1) NOT NULL DEFAULT '0',
  `Tue` tinyint(1) NOT NULL DEFAULT '0',
  `Wed` tinyint(1) NOT NULL DEFAULT '0',
  `Thu` tinyint(1) NOT NULL DEFAULT '0',
  `Fri` tinyint(1) NOT NULL DEFAULT '0',
  `Sat` tinyint(1) NOT NULL DEFAULT '0',
  `Sun` tinyint(1) NOT NULL DEFAULT '0',
  `time` time NOT NULL,
  `holiday` enum('on','off') CHARACTER SET utf8 NOT NULL DEFAULT 'off',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- テーブルの構造 `course`
--

CREATE TABLE IF NOT EXISTS `course` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(30) NOT NULL,
  `note` varchar(30) NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=3 ;

--
-- テーブルのデータのダンプ `course`
--

INSERT INTO `course` (`id`, `name`, `note`, `create`, `modified`) VALUES
(1, '月水金', '', '2012-01-01 00:00:00', '2012-01-01 00:00:00'),
(2, '火木土', '', '2012-01-01 00:00:00', '2012-01-01 00:00:00');

-- --------------------------------------------------------

--
-- テーブルの構造 `data`
--

CREATE TABLE IF NOT EXISTS `data` (
  `id` int(11) NOT NULL,
  `idm` char(16) CHARACTER SET ascii NOT NULL,
  `pmm` char(16) CHARACTER SET ascii NOT NULL,
  `codabar` int(11) NOT NULL,
  `create_t` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- テーブルのデータのダンプ `data`
--


-- --------------------------------------------------------

--
-- テーブルの構造 `event`
--

CREATE TABLE IF NOT EXISTS `event` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` date NOT NULL,
  `note` varchar(100) NOT NULL,
  `arbitrary` tinyint(1) NOT NULL,
  `origin` date NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 COMMENT='イベントを登録します' AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- テーブルの構造 `holiday`
--

CREATE TABLE IF NOT EXISTS `holiday` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `date` date NOT NULL,
  `desc` varchar(20) NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 AUTO_INCREMENT=1 ;

-- --------------------------------------------------------

--
-- テーブルの構造 `order`
--

CREATE TABLE IF NOT EXISTS `order` (
  `id` int(11) NOT NULL,
  `user_id` mediumint(5) NOT NULL,
  `product_id` mediumint(5) NOT NULL,
  `quantity` mediumint(5) NOT NULL,
  `unit` mediumint(5) NOT NULL,
  `price` int(10) NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- --------------------------------------------------------

--
-- テーブルの構造 `parents`
--

CREATE TABLE IF NOT EXISTS `parents` (
  `id` int(11) NOT NULL,
  `login` char(16) CHARACTER SET ascii NOT NULL,
  `password` char(16) CHARACTER SET ascii NOT NULL,
  `name` varchar(10) NOT NULL,
  `email` char(64) CHARACTER SET ascii NOT NULL,
  `tel` char(13) CHARACTER SET ascii NOT NULL,
  `fax` char(13) CHARACTER SET ascii NOT NULL,
  `address` varchar(128) NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

--
-- テーブルのデータのダンプ `parents`
--

INSERT INTO `parents` (`id`, `login`, `password`, `name`, `email`, `tel`, `fax`, `address`, `create`, `modified`) VALUES
(1, 'mama', 'password', 'まま', 'mama@gmail.com', '0312121212', '0323232323', 'とうきょうとめぐろくめぐろほんちょう１２３４－４５６５６－８９７０　めぐろほんちょうがーでん１２３４', '0000-00-00 00:00:00', '0000-00-00 00:00:00'),
(2, 'papa', 'password', 'ぱぱ', 'papa@kidssafe.me', '05099998888', '05088887777', '裏山鹿', '0000-00-00 00:00:00', '0000-00-00 00:00:00'),
(3, 'jiji', 'password', 'jijii', 'jijii@gmail.com', '0313456789', '0312345678', 'とうきょうとしながわくにしこやまほんちょう１２－３４－５６　ぱーくはいつひるずたわー１２３４－５６７８', '2012-04-03 00:12:30', '2012-04-03 00:12:33');

-- --------------------------------------------------------

--
-- テーブルの構造 `product`
--

CREATE TABLE IF NOT EXISTS `product` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(100) NOT NULL,
  `unit` mediumint(5) NOT NULL,
  `note` varchar(100) NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=4 ;

--
-- テーブルのデータのダンプ `product`
--

INSERT INTO `product` (`id`, `name`, `unit`, `note`, `create`, `modified`) VALUES
(1, '6級テキスト', 0, '', '2012-01-01 00:00:00', '2012-01-01 00:00:00'),
(2, '5級テキスト', 0, '', '2012-01-01 00:00:00', '2012-01-01 00:00:00'),
(3, 'ぶんちんのぶんちゃん', 100, '', '0000-00-00 00:00:00', '0000-00-00 00:00:00');

-- --------------------------------------------------------

--
-- テーブルの構造 `timetable`
--

CREATE TABLE IF NOT EXISTS `timetable` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `course` mediumint(5) NOT NULL,
  `name` varchar(16) NOT NULL,
  `day` varchar(8) NOT NULL,
  `begin_t` time NOT NULL,
  `end_t` time NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=33 ;

--
-- テーブルのデータのダンプ `timetable`
--

INSERT INTO `timetable` (`id`, `course`, `name`, `day`, `begin_t`, `end_t`, `create`, `modified`) VALUES
(1, 0, '0', '月', '15:00:00', '16:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(2, 0, '1', '月', '16:00:00', '17:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(3, 0, '2', '月', '17:00:00', '18:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(4, 0, '3', '月', '18:00:00', '19:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(5, 0, '0', '金', '15:00:00', '16:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(6, 0, '1', '金', '16:00:00', '17:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(7, 0, '2', '金', '17:00:00', '18:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(8, 0, '3', '金', '18:00:00', '19:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(9, 0, '4', '金', '19:00:00', '20:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(10, 0, '1', '土', '10:00:00', '11:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(11, 0, '2', '土', '11:00:00', '12:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(12, 0, '3', '土', '15:00:00', '16:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(13, 0, '4', '土', '16:00:00', '17:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(14, 0, '5', '土', '17:00:00', '18:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(15, 0, '6', '土', '18:00:00', '19:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(16, 0, '7', '土', '19:00:00', '20:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(17, 1, '0', '火', '15:00:00', '16:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(18, 1, '1', '火', '16:00:00', '17:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(19, 1, '2', '火', '17:00:00', '18:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(20, 1, '3', '火', '18:00:00', '19:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(21, 1, '0', '木', '15:00:00', '16:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(22, 1, '1', '木', '16:00:00', '17:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(23, 1, '2', '木', '17:00:00', '18:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(24, 1, '3', '木', '18:00:00', '19:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(25, 1, '4', '木', '19:00:00', '20:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(26, 1, '1', '土', '10:00:00', '11:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(27, 1, '2', '土', '11:00:00', '12:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(28, 1, '3', '土', '15:00:00', '16:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(29, 1, '4', '土', '16:00:00', '17:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(30, 1, '5', '土', '17:00:00', '18:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(31, 1, '6', '土', '18:00:00', '19:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11'),
(32, 1, '7', '土', '19:00:00', '20:00:00', '2012-02-02 08:11:11', '2012-02-02 08:11:11');

-- --------------------------------------------------------

--
-- テーブルの構造 `users`
--

CREATE TABLE IF NOT EXISTS `users` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `login` char(16) CHARACTER SET ascii NOT NULL,
  `password` char(16) CHARACTER SET ascii NOT NULL,
  `user` varchar(16) NOT NULL,
  `status` enum('active','inactive','graduation') NOT NULL DEFAULT 'active',
  `idm` char(16) CHARACTER SET ascii NOT NULL,
  `pmm` char(16) CHARACTER SET ascii NOT NULL,
  `codabar` char(10) NOT NULL,
  `course` smallint(5) NOT NULL,
  `parents` smallint(5) NOT NULL,
  `privilege` smallint(5) NOT NULL,
  `create` datetime NOT NULL,
  `modified` datetime NOT NULL,
  `graduation` datetime NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=utf8 AUTO_INCREMENT=9 ;

--
-- テーブルのデータのダンプ `users`
--

INSERT INTO `users` (`id`, `login`, `password`, `user`, `status`, `idm`, `pmm`, `codabar`, `course`, `parents`, `privilege`, `create`, `modified`, `graduation`) VALUES
(1, 'admin', 'password', 'admin', 'active', '111', '1111', 'A010001B', 1, 0, 1, '2012-01-01 00:00:00', '2012-01-01 00:00:00', '0000-00-00 00:00:00'),
(2, 'ayano', 'password', 'ayano', 'active', '222', '2222', 'A010002C', 1, 1, 0, '2012-01-01 00:00:00', '2012-01-01 00:00:00', '0000-00-00 00:00:00'),
(3, 'haru', 'password', 'haru', 'active', '333', '3333', 'A010003D', 1, 2, 0, '2012-01-01 00:00:00', '2012-01-01 00:00:00', '0000-00-00 00:00:00'),
(4, 'hana', 'password', 'hana', 'active', '444', '4444', 'A010004D', 2, 2, 0, '2012-01-01 00:00:00', '2012-01-01 00:00:00', '0000-00-00 00:00:00'),
(5, 'maru', 'password', 'maru', 'active', '555', '5555', 'A010005E', 2, 3, 0, '0000-00-00 00:00:00', '0000-00-00 00:00:00', '0000-00-00 00:00:00'),
(6, 'yoshimi', 'password', 'yoshimi', 'active', '666', '6666', 'A010006F', 2, 3, 0, '0000-00-00 00:00:00', '2012-04-08 23:29:11', '0000-00-00 00:00:00');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
