-- phpMyAdmin SQL Dump
-- version 4.6.4
-- https://www.phpmyadmin.net/
--
-- Φιλοξενητής: 127.0.0.1
-- Χρόνος δημιουργίας: 13 Αυγ 2017 στις 12:36:40
-- Έκδοση διακομιστή: 5.7.14
-- Έκδοση PHP: 5.6.25

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Βάση δεδομένων: `web_courier`
--

-- --------------------------------------------------------

--
-- Δομή πίνακα για τον πίνακα `connects`
--

CREATE TABLE `connects` (
  `hub_a` int(3) NOT NULL,
  `hub_b` int(3) NOT NULL,
  `cost_ab` int(11) NOT NULL,
  `eta_ab` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Άδειασμα δεδομένων του πίνακα `connects`
--

INSERT INTO `connects` (`hub_a`, `hub_b`, `cost_ab`, `eta_ab`) VALUES
(1, 7, 8, 1),
(8, 2, 4, 2),
(8, 7, 10, 1),
(8, 9, 15, 1),
(9, 8, 15, 1),
(9, 7, 10, 1),
(9, 4, 3, 1),
(4, 9, 3, 1),
(4, 7, 5, 1),
(4, 3, 1, 1),
(4, 5, 1, 1),
(3, 7, 2, 1),
(5, 4, 1, 1),
(5, 6, 3, 1),
(6, 5, 3, 1),
(6, 7, 2, 1),
(6, 2, 2, 1),
(2, 6, 2, 1),
(2, 7, 3, 1),
(2, 8, 4, 2),
(7, 1, 8, 1),
(7, 9, 10, 1),
(7, 4, 5, 1),
(7, 3, 2, 1),
(7, 6, 2, 1),
(7, 2, 3, 1),
(7, 8, 10, 1);

-- --------------------------------------------------------

--
-- Δομή πίνακα για τον πίνακα `local_store`
--

CREATE TABLE `local_store` (
  `id` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `address` varchar(100) NOT NULL,
  `addr_num` int(11) NOT NULL,
  `city` varchar(100) NOT NULL,
  `zip_code` int(11) NOT NULL,
  `phone_num` BIGINT NOT NULL,
  `coords` text NOT NULL,
  `transit_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Άδειασμα δεδομένων του πίνακα `local_store`
--

INSERT INTO `local_store` (`id`, `name`, `address`, `addr_num`, `city`, `zip_code`, `phone_num`, `coords`, `transit_id`) VALUES
(1, 'Polygyros', 'Πολυτεχνείου', 16, 'Θεσσαλονίκη', 55134, 2313330822, '40.3844563733118,23.441407748767233', 4),
(2, 'Xalandri', 'Σωκράτους', 9, 'Αθήνα', 15232, 2132100513, '38.019925544541735,23.80023833202813', 7),
(3, 'Araxovitika', 'Παλιά ΕΟ Κορίνθου-Πατρών', 162, 'Πάτρα', 26504, 2613613350, '38.32473610644483,21.8542299776027', 6),
(4, 'Chania', 'Κνωσσού', 32, 'Κρήτη', 73134, 2821346601, '35.5075527,24.007230', 8),
(5, 'Karditsa', 'Αγίου Δημητρίου', 18, 'Καρδίτσα', 43100, 2441350100, '39.364708,21.9021186', 3);

-- --------------------------------------------------------

--
-- Δομή πίνακα για τον πίνακα `transit_hub`
--

CREATE TABLE `transit_hub` (
  `id` int(11) NOT NULL,
  `name` varchar(100) NOT NULL,
  `address` varchar(100) NOT NULL,
  `addr_num` int(11) NOT NULL,
  `city` varchar(100) NOT NULL,
  `zip_code` int(11) NOT NULL,
  `phone_num` BIGINT NOT NULL,
  `coords` text NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Άδειασμα δεδομένων του πίνακα `transit_hub`
--

INSERT INTO `transit_hub` (`id`, `name`, `address`, `addr_num`, `city`, `zip_code`, `phone_num`, `coords`) VALUES
(1, 'Mytilini', 'Κυκλάδων', 53, 'Μυτιλήνη', 81100, 2251042236, '39.0916996,26.53472899999997'),
(2, 'Kalamata', 'Καλλιπατειρας', 5, 'Καλαμάτα', 24100, 2721095841, '37.04202439999999,22.120971699999927'),
(3, 'Larisa', 'Σωράνου,Γρ.Λαμπράκη', 21, 'Λάρισα', 41447, 2413506100, '39.6437676,22.412109399999963'),
(4, 'Thessaloniki', 'Σοφοκλή Βενιζέλου', 56, 'Θεσσαλονίκη', 55134, 2310556131, '40.672306,22.939453100000037'),
(5, 'Ioannina', '25ης Μαρτίου', 124, 'Ιωάννινα', 45332, 2651087108, '39.68552191166493,20.86585610167458'),
(6, 'Patras', 'Μυκηνών,Κολχίδος', 4, 'Πάτρα', 26223, 2613613200, '38.22515829074799,21.73951464176821'),
(7, 'Aspropyrgos', 'Σαλαμίνος', 36, 'Αθήνα', 19300, 2131601370, '38.05620054400559,23.58697589325402'),
(8, 'Heraklion', 'Παρ. 3η Ποσειδώνος', 5, 'Ηράκλειο', 71303, 2813410410, '35.33977430000003,25.14770509999994'),
(9, 'Alexandroupoli', 'Αγίου Δημητρίου', 32, 'Αλεξανδρούπολη', 68100, 2551355328, '40.856589,25.863566799999944');

-- --------------------------------------------------------

--
-- Δομή πίνακα για τον πίνακα `users`
--

CREATE TABLE `users` (
  `uname` varchar(100) NOT NULL,
  `passw` varchar(256) NOT NULL,
  `role` enum('transit_employee','store_employee','admin') NOT NULL,
  `store` varchar(100) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Άδειασμα δεδομένων του πίνακα `users`
--

INSERT INTO `users` (`uname`, `passw`, `role`, `store`) VALUES
('admin', '$2y$10$kFHVIuzvJVErRduyTBNJxuVbz1Ld1fe8et9eblLe00BZbxA.sSJ8m', 'admin', 'NONE'),
('Αθηνά', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Kalamata'),
('Αλέξανδρος', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Aspropyrgos'),
('Άννα', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Alexandroupoli'),
('Άρης', '$2y$10$fKAqERFpeMoTiOR9tWNWgOqiBMFqjKjCFQahUG33AmeL.1Z770U0C', 'store_employee', 'Chania'),
('Βαγγέλης', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Larisa'),
('Γιώργος', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Patras'),
('ΓιώργοςΓ', '$2y$10$SWecu1OjCgfAdfhyXQqX4OJBqPQfdCFP1Etb1/SEAEeYQKqS4zW5W', 'store_employee', 'Araxovitika'),
('Ελένη', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Ioannina'),
('Κατερίνα', '$2y$10$e1cH4MoV1e3s5slRe0.3D.cdxVuEqdk.cUfwGb0MGVhF0Uh8/.Gn.', 'store_employee', 'Karditsa'),
('Κώστας', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Mytilini'),
('ΚώσταςΓ', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'store_employee', 'Xalandri'),
('Μαρία', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'transit_employee', 'Heraklion'),
('Νίκος', '$2y$10$x3YadhahmMIbXHZXUEzs5.KF3wzxFfacutiwt6szjiRXiup1dEO0m', 'transit_employee', 'Thessaloniki'),
('Ρία', '$2y$10$NE4QyYKp3KrXH8QTMycLP.x/dVtnQtInWAWvavIuJPVOh.K94jyba', 'store_employee', 'Polygyros');

-- --------------------------------------------------------

--
-- Δομή πίνακα για τον πίνακα `web_order`
--

CREATE TABLE `web_order` (
  `tracking_num` varchar(100) NOT NULL,
  `typeof` enum('express','standard') NOT NULL,
  `current_point` varchar(100) NOT NULL,
  `route` text,
  `cost` int(11) NOT NULL,
  `eta` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Άδειασμα δεδομένων του πίνακα `web_order`
--

INSERT INTO `web_order` (`tracking_num`, `typeof`, `current_point`, `route`, `cost`, `eta`) VALUES
('AR1503673554PO', 'express', 'Patras', 'Araxovitika,Patras,Ioannina,Thessaloniki,Polygyros', 6, 2),
('PO1502035617XA', 'standard', 'Larisa', 'Polygyros,Thessaloniki,Larisa,Aspropyrgos,Xalandri', 5, 2);

--
-- Ευρετήρια για άχρηστους πίνακες
--

--
-- Ευρετήρια για πίνακα `connects`
--
ALTER TABLE `connects`
  ADD KEY `ida_connects` (`hub_a`),
  ADD KEY `idb_connects` (`hub_b`);

--
-- Ευρετήρια για πίνακα `local_store`
--
ALTER TABLE `local_store`
  ADD PRIMARY KEY (`id`),
  ADD KEY `fk_local_store_transit_hub` (`transit_id`);

--
-- Ευρετήρια για πίνακα `transit_hub`
--
ALTER TABLE `transit_hub`
  ADD PRIMARY KEY (`id`);

--
-- Ευρετήρια για πίνακα `users`
--
ALTER TABLE `users`
  ADD PRIMARY KEY (`uname`);

--
-- Ευρετήρια για πίνακα `web_order`
--
ALTER TABLE `web_order`
  ADD PRIMARY KEY (`tracking_num`);

--
-- AUTO_INCREMENT για άχρηστους πίνακες
--

--
-- AUTO_INCREMENT για πίνακα `local_store`
--
ALTER TABLE `local_store`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=6;
--
-- AUTO_INCREMENT για πίνακα `transit_hub`
--
ALTER TABLE `transit_hub`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
--
-- Περιορισμοί για άχρηστους πίνακες
--

--
-- Περιορισμοί για πίνακα `connects`
--
ALTER TABLE `connects`
  ADD CONSTRAINT `fk_connects_transit_hub_a` FOREIGN KEY (`hub_a`) REFERENCES `transit_hub` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION,
  ADD CONSTRAINT `fk_connects_transit_hub_b` FOREIGN KEY (`hub_b`) REFERENCES `transit_hub` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION;

--
-- Περιορισμοί για πίνακα `local_store`
--
ALTER TABLE `local_store`
  ADD CONSTRAINT `fk_local_store_transit_hub` FOREIGN KEY (`transit_id`) REFERENCES `transit_hub` (`id`) ON DELETE CASCADE ON UPDATE NO ACTION;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
