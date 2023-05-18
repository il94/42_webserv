#!/usr/bin/env python

import cgi
import os

# Fonction pour générer le formulaire de connexion
def generate_login_form():
    print("Content-type: text/html")
    print("")
    print("<html>")
    print("<body>")
    print("<h1>Connexion</h1>")
    print('<form method="post" action="login.cgi">')
    print('Nom d\'utilisateur: <input type="text" name="username"><br>')
    print('Mot de passe: <input type="password" name="password"><br>')
    print('<input type="submit" value="Connexion">')
    print('</form>')
    print("</body>")
    print("</html>")

# Fonction pour générer la page de déconnexion
def generate_logout_page():
    print("Content-type: text/html")
    print("Set-Cookie: session=; expires=Thu, 01 Jan 1970 00:00:00 GMT")  # Supprimer le cookie de session
    print("")
    print("<html>")
    print("<body>")
    print("<h1>Déconnexion</h1>")
    print("<p>Vous êtes déconnecté.</p>")
    print("</body>")
    print("</html>")

# Vérifier si le cookie de session existe
def check_session():
    if 'HTTP_COOKIE' in os.environ:
        cookies = os.environ['HTTP_COOKIE'].split(';')
        for cookie in cookies:
            name, value = cookie.strip().split('=')
            if name == 'session' and value == 'loggedin':
                return True
    return False

# Vérifier les données de connexion
def check_credentials(username, password):
    # Vérification factice des données (exemple)
    if username == 'admin' and password == 'admin':
        return True
    return False

# Récupérer les données POST
form = cgi.FieldStorage()

# Vérifier si l'utilisateur est déjà connecté
if check_session():
    # Afficher la page de déconnexion si l'utilisateur est déjà connecté
    generate_logout_page()
else:
    # Vérifier si les données de connexion ont été soumises
    if 'username' in form and 'password' in form:
        username = form['username'].value
        password = form['password'].value

        # Vérifier les informations de connexion
        if check_credentials(username, password):
            # Créer un cookie de session
            print("Content-type: text/html")
            print("Set-Cookie: session=loggedin")  # Définir le cookie de session
            print("")
            print("<html>")
            print("<body>")
            print("<h1>Connexion réussie</h1>")
            print("<p>Bienvenue, {}!</p>".format(username))
            print("<p><a href=\"login.cgi\">Se déconnecter</a></p>")
            print("</body>")
            print("</html>")
        else:
            # Afficher à nouveau le formulaire de connexion avec un message d'erreur
            generate_login_form()
            print("<p>Identifiants invalides. Veuillez réessayer.</p>")
    else:
        # Afficher le formulaire de connexion
        generate_login_form()
