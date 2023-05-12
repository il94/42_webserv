#!/usr/bin/bash

echo $QUERY_STRING > "$UPLOAD_PATH$FILE_NAME"

echo "$UPLOAD_PATH$FILE_NAME"
echo"Content-Type: text/html\r\n\r\n";

echo "<html><head><title>uploaded</title></head><body>\r\n";
echo "<h1>NICELY UPLOADED YEAHHHHH</h1>\r\n";
