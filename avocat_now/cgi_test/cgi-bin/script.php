#!/usr/bin/php-cgi
<?php
header("Content-type: text/html");

echo "<html><head><title>Réponse du serveur</title></head><body>";
echo "<h1>Réponse du serveur</h1>";

$nom = $_POST['nom'];
$email = $_POST['email'];
$message = $_POST['message'];

echo "<p>Merci pour votre message, $nom ($email) !</p>";
echo "<p>Voici votre message :</p>";
echo "<blockquote>$message</blockquote>";

echo "</body></html>";
?>
