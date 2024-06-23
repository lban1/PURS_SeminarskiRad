DROP DATABASE IF EXISTS Projektni;
CREATE DATABASE Projektni;
USE Projektni;

DROP USER IF EXISTS app;
CREATE USER app@'%' IDENTIFIED BY '1234';
GRANT SELECT, INSERT, UPDATE, DELETE ON Projektni.* TO app@'%';

CREATE TABLE temperatura (
	id INT PRIMARY KEY AUTO_INCREMENT,
	datum DATETIME,
	vrijednost INT
);

INSERT INTO temperatura (datum,vrijednost)
VALUES ('2023-10-10', '23'), 
		('2023-10-11', '20');

CREATE TABLE vlaga (
	id INT PRIMARY KEY AUTO_INCREMENT,
	datum DATETIME,
	vrijednost INT
);

INSERT INTO vlaga (datum,vrijednost)
VALUES ('2023-10-10', '87'), 
		('2023-10-11', '77');

CREATE TABLE temperaturavlaga (
	id INT PRIMARY KEY AUTO_INCREMENT,
	datum DATETIME,
	vrijednosttemp INT,
    vrijednostvlaga INT
);

INSERT INTO temperaturavlaga (datum,vrijednosttemp,vrijednostvlaga)
VALUES ('2023-10-10', '23','56');

CREATE TABLE ovlasti (
	id INT PRIMARY KEY AUTO_INCREMENT,
	naziv VARCHAR(100)
);

INSERT INTO ovlasti (naziv)
VALUES ('Administrator'), 
		('Korisnik');

CREATE TABLE korisnik (
	id INT PRIMARY KEY AUTO_INCREMENT,
	ime CHAR(50),
    prezime CHAR(50),
    username VARCHAR(50),
    password VARCHAR(50),
    id_ovlasti INT,
    FOREIGN KEY (id_ovlasti) REFERENCES ovlasti(id) 
);

INSERT INTO korisnik (ime, prezime, username, password, id_ovlasti)
VALUES ('Ladislav', 'Kovač','lkovac','1234','1'),
		('Valentina', 'Ilić','vilic','abcd','1');

CREATE TABLE korisnikove_temperature (
	id_korisnika INT,
	id_temperature INT,
    FOREIGN KEY (id_korisnika) REFERENCES korisnik(id),
    FOREIGN KEY (id_temperature) REFERENCES temperatura(id),
    PRIMARY KEY (id_korisnika, id_temperature)
);

INSERT INTO korisnikove_temperature (id_korisnika, id_temperature)
VALUES ('1', '1'),
		('2', '1');
