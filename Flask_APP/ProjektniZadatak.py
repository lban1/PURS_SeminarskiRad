from flask import Flask, request, url_for, redirect, make_response, render_template, session, g, jsonify
import MySQLdb
from datetime import datetime
from functools import wraps

app = Flask("Prva flask aplikacija")

@app.before_request
def before_request_func():
    g.connection = MySQLdb.connect(host="localhost", user="root", passwd="Projektni123!", db="Projektni")
    g.cursor = g.connection.cursor()


@app.get('/')
def home_page():
    id_param = request.args.get('id')
    
    if id_param == '1' or id_param is None:
        g.cursor.execute("SELECT datum, vrijednost FROM temperatura ORDER BY id DESC LIMIT 1")
        zadnja_vrijednost_temp = g.cursor.fetchone()
        naslov = 'TEMPERATURA'
        
        if zadnja_vrijednost_temp:
            vrijeme, vrijednost = zadnja_vrijednost_temp
        else:
            vrijeme, vrijednost = None, None

    elif id_param == '2':
        g.cursor.execute("SELECT datum, vrijednost FROM vlaga ORDER BY id DESC LIMIT 1")
        zadnja_vrijednost_vlaga = g.cursor.fetchone()
        naslov = 'VLAGA'

        if zadnja_vrijednost_vlaga:
            vrijeme, vrijednost = zadnja_vrijednost_vlaga
        else:
            vrijeme, vrijednost = None, None

    else:
        vrijeme, vrijednost = None, None
        naslov = None

    response = render_template('index.html',username = session['username'], title='Pocetna stranica', title2 = naslov, vrijeme_in = vrijeme, vrijednost_in = vrijednost)
    return response, 200

@app.route('/DataBase')
def database_page():
    if session['id_ovlasti'] != 1:
        return redirect(url_for('login'))
    else:
        #g.cursor.execute(render_template('read_vrijednosti.sql', table = 'temperatura'))
        g.cursor.execute("SELECT * FROM temperatura INNER JOIN vlaga ON temperatura.datum = vlaga.datum")
        lista_podataka = g.cursor.fetchall()

        return render_template('index2.html',username = session['username'], title = 'Database',title2 = 'BAZA PODATAKA', lista = lista_podataka)

'''@app.post('/temperatura')
def put_temperatura():
    global temperatura
    response = make_response()

    if request.json.get('temperatura') is not None:
        query = render_template('write_temperaturavlaga.sql', ocitanje=request.json.get('temperatura'))
        g.cursor.execute(query)

        response.data = 'Uspješno ste postavili temperaturu'
        response.status_code = 201
    else:
        response.data = 'Niste napisali ispravan ključ'
        response.status_code = 404
    return response'''


@app.after_request
def after_request_func(response):
    g.connection.commit()
    g.connection.close()
    return response


@app.route('/')
def index():
    return render_template('index.html',username = session['username'], naslov='Početna stranica')


@app.route('/login', methods=['GET', 'POST'])
def login():
    if request.method == 'POST':
        username = request.form.get('username')
        password = request.form.get('password')
        g.cursor.execute("SELECT username, id_ovlasti FROM korisnik WHERE username = %s AND password = %s", (username, password))
        korisnik = g.cursor.fetchone()
        if korisnik:
            session['username'] = korisnik[0]
            session['id_ovlasti'] = korisnik[1]
            return redirect(url_for('index'))
        else:
            return render_template('login.html', naslov="Stranica za prijavu", login_poruka="Uneseni su neispravni podaci")
    else:
        return render_template('login.html', naslov="Stranica za prijavu")


@app.route('/logout')
def logout():
    session.pop('username', None)
    session.pop('id_ovlasti', None)
    return redirect(url_for('login'))


@app.route('/temperatura', methods=['POST'])
def temperatura():
    data = request.get_json()
    temp = data.get('temperatura')
    vlaga = data.get('vlaga')
    if temp is None or vlaga is None:
        return 'Niste upisali ispravan ključ', 404
    else:
        query_temp = "INSERT INTO temperatura (vrijednost, datum) VALUES (%s, %s)"
        query_vlaga = "INSERT INTO vlaga (vrijednost, datum) VALUES (%s, %s)"
        current_time = datetime.now()
        g.cursor.execute(query_temp, (temp, current_time))
        g.cursor.execute(query_vlaga, (vlaga, current_time))
        print(f'Postavljena temperatura je {temp}, vlaga je {vlaga}, a vrijeme je {current_time}')
        return 'Vrijednost uspiješno postavljena.', 201


@app.route('/latest_value', methods=['GET'])
def latest_value():
    g.cursor.execute("SELECT datum, vrijednost FROM temperatura ORDER BY id DESC LIMIT 1")
    latest_temperature = g.cursor.fetchone()
    g.cursor.execute("SELECT datum, vrijednost FROM vlaga ORDER BY id DESC LIMIT 1")
    latest_humidity = g.cursor.fetchone()

    if latest_temperature and latest_humidity:
        json_response = {
            'vrijeme_temp': str(latest_temperature[0]),
            'temperatura': latest_temperature[1],
            'vrijeme_vlaga': str(latest_humidity[0]),
            'vlaga': latest_humidity[1]
        }
    else:
        json_response = {
            'vrijeme_temp': None,
            'temperatura': None,
            'vrijeme_vlaga': None,
            'vlaga': None
        }
    return jsonify(json_response)


app.secret_key = '_5#y2L"F4Q8z-n-xec]/'


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=80)