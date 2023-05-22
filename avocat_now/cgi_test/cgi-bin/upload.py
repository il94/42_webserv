#!/usr/bin/env python

import cgi

print("Content-Type: text/html\r\n\r\n")
print("""<html>\n\
<head>\n\
	<title>Upload Success</title>\n\
	<style>\n\
		body {\n\
		background-color: #f0f0f0;\n\
		font-family: Arial, sans-serif;\n\
		text-align: center;\n\
		padding: 20px;\n\
		}\n\
	h1 {\n\
	color: #333333;\n\
	}\n\
	p {\n\
	  color: #666666;\n\
	}\n\
	.success {\n\
	  color: #009900;\n\
	  font-size: 20px;\n\
	  margin-top: 40px;\n\
	}\n\
	.uploaded-images {\n\
	  margin-top: 40px;\n\
	}\n\
	.uploaded-images img {\n\
	  margin: 10px;\n\
	  max-width: 200px;\n\
	  max-height: 200px;\n\
	}\n\
  </style>\n\
</head>\n\
<body>\n\
  <h1>Upload</h1>\n\
  <div class="success">\n\
    <p>uploaded successfully!</p>\n\
  </div>""")


