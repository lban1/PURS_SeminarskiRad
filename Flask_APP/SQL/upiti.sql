SELECT * FROM temperatura;
SELECT * FROM vlaga;
SELECT * FROM temperatura ORDER BY id DESC LIMIT 1;

SELECT * FROM korisnik;

SELECT ime, prezime, naziv AS naziv_ovlasti FROM korisnik
INNER JOIN ovlasti ON korisnik.id_ovlasti = ovlasti.id;

SELECT * FROM korisnik WHERE username = 'kkolar' OR password = '12ab';

SELECT ime, prezime, vrijednost AS vrijednost_temperature FROM korisnik
LEFT JOIN korisnikove_temperature ON korisnikove_temperature.id_korisnika = korisnik.id
LEFT JOIN temperatura ON korisnikove_temperature.id_temperature = temperatura.id
WHERE id_korisnika = 1;