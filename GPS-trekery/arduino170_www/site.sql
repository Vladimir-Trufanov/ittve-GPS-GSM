-- phpMyAdmin SQL Dump
-- version 3.5.1
-- http://www.phpmyadmin.net
--
-- Хост: 127.0.0.1
-- Время создания: Окт 09 2015 г., 09:48
-- Версия сервера: 5.5.25
-- Версия PHP: 5.3.13

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- База данных: `gps_tracker`
--

-- --------------------------------------------------------

--
-- Структура таблицы `cars`
--

CREATE TABLE IF NOT EXISTS `cars` (
  `id` int(9) NOT NULL AUTO_INCREMENT,
  `owner` int(9) NOT NULL,
  `mark` varchar(30) NOT NULL,
  `number` varchar(30) NOT NULL,
  `id_driver` int(9) DEFAULT '0',
  `data1` datetime NOT NULL,
  `data2` datetime NOT NULL,
  `status` set('yes','no') DEFAULT 'yes',
  UNIQUE KEY `id` (`id`),
  KEY `data1` (`data1`)
) ENGINE=MyISAM  DEFAULT CHARSET=cp1251 AUTO_INCREMENT=4 ;

--
-- Дамп данных таблицы `cars`
--

INSERT INTO `cars` (`id`, `owner`, `mark`, `number`, `id_driver`, `data1`, `data2`, `status`) VALUES
(1, 4, 'ВАЗ 2110', 'Е123КХ26RUS белый', 1, '2011-01-29 09:32:07', '2011-01-29 09:31:36', 'yes'),
(2, 4, 'ВАЗ 2106', 'С123КН26RUS зеленый', 3, '2011-01-29 09:32:20', '2011-01-29 09:15:57', 'yes'),
(3, 2, 'ВАЗ 2114', 'Е567КН26RUS сиреневый', 2, '2011-01-29 09:31:53', '2010-10-27 16:28:12', 'yes');

-- --------------------------------------------------------

--
-- Структура таблицы `gps_tracker`
--

CREATE TABLE IF NOT EXISTS `gps_tracker` (
  `id` int(9) NOT NULL AUTO_INCREMENT,
  `id_avto` int(2) NOT NULL,
  `lat` float(10,5) NOT NULL,
  `lon` float(10,5) NOT NULL,
  `data` datetime NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `id` (`id`)
) ENGINE=MyISAM  DEFAULT CHARSET=cp1251 AUTO_INCREMENT=139 ;

--
-- Дамп данных таблицы `gps_tracker`
--

INSERT INTO `gps_tracker` (`id`, `id_avto`, `lat`, `lon`, `data`) VALUES
(126, 1, 44.04069, 43.05122, '2015-07-05 00:48:14'),
(127, 1, 44.04169, 43.05222, '2015-07-05 02:00:00'),
(128, 1, 44.04269, 43.05322, '2015-07-05 06:00:00'),
(129, 2, 44.04069, 43.05122, '2015-07-05 00:48:14'),
(130, 1, 44.04069, 43.05322, '2015-07-05 00:48:14'),
(131, 1, 44.04169, 43.05522, '2015-07-05 02:00:00'),
(132, 1, 44.04269, 43.05622, '2015-07-05 02:00:00'),
(133, 1, 44.04369, 43.05722, '2015-07-05 02:00:00'),
(134, 1, 44.04469, 43.05722, '2015-07-05 02:00:00'),
(135, 1, 44.04569, 43.05722, '2015-07-05 02:00:00'),
(136, 1, 44.04669, 43.05622, '2015-07-05 02:00:00'),
(137, 1, 44.04769, 43.05522, '2015-07-05 02:00:00'),
(138, 1, 44.04769, 43.05622, '2015-07-05 02:00:00');

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
