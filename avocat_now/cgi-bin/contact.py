#!/usr/bin/env python

import cgi

form = cgi.FieldStorage()

username = form.getvalue("username")
email = form.getvalue("email")
message = form.getvalue("message")

print("Content-Type: text/html\r\n\r\n")

with open('avocat_now/cgi-bin/contact_content/contact_response.html', 'r') as f:
	for ligne in f:
		if username and "{username}" in ligne:
			print(ligne.replace("{username}", username))
		elif email and "{email}" in ligne:
			print(ligne.replace("{email}", email))
		elif message and "{message}" in ligne:
			print(ligne.replace("{message}", message))
		else:
			print(ligne, end='')

