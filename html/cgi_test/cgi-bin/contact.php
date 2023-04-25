#!/usr/bin/env php

<?php
echo("Content-Type: text/html\r\n\r\n");

echo "<html><head><title>Reponse du serveur</title></head><body>\n";
echo "<h1>Reponse du serveur</h1>\n";

parse_str(file_get_contents('php://stdin'), $_POST);
$nom = $_POST["nom"];
$email = $_POST["email"];
$message = $_POST["message"];

echo "<p>Merci pour votre message, $nom ($email) !\n</p>";
echo "<p>Voici votre message :</p>\n";
echo "<blockquote>$message</blockquote>\n";

echo "</body></html>";
?>
