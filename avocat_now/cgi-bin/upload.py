#!/usr/bin/env python

import cgi

print("Content-Type: text/html\r\n\r\n")
print("")

with open('avocat_now/cgi-bin/upload_content/upload_success.html', 'r') as f:
	for ligne2 in f:
		print(ligne2, end='')
