#!/usr/bin/env python

import cgi

print("Content-type: text/html\n\n")
print("<html><head><title>Reponse du serveur</title></head><body>")
print("<h1>Reponse du serveur</h1>")

form = cgi.FieldStorage()
nom = form.getvalue("nom")
email = form.getvalue("email")
message = form.getvalue("message")

print("<p>Merci pour votre message, {} ({}) !</p>".format(nom, email))
print("<p>Voici votre message :</p>")
print("<blockquote>{}</blockquote>".format(message))

print("</body></html>")