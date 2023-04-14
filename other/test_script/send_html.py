# Ouverture du fichier en mode lecture
with open("../html/index.html", "r") as fichier:
    # Lecture du contenu du fichier
    contenu = fichier.read()

# Affichage du contenu dans le terminal
print(contenu)