#ifndef CHUCK_HPP
#define CHUCK_HPP

#include <iostream>
#include <random>
#include <string>
#include <vector>

class Chuck
{

public:
    std::vector<std::string> CHUCK_FACTS = {
        "Quand Chuck Norris entre dans une pièce, il allume la lumière avec un regard.",
        "Chuck Norris connaît le dernier chiffre de Pi.",
        "Google ne cherche pas. Il demande à Chuck Norris.",
        "Le GMT est calé sur l’heure de Chuck Norris.",
        "Chuck Norris peut fermer des onglets récalcitrants dans la vraie vie.",
        "Les mots de passe de Chuck Norris se connectent d’eux-mêmes.",
        "Le code de Chuck Norris compile par respect.",
        "Chuck Norris a déjà fini Skyrim… en cinématique d’intro.",
        "Chuck Norris peut faire un nœud avec un câble USB-C.",
        "Les exceptions remercient Chuck Norris avant de se lever.",
        "Chuck Norris peut diviser par zéro sans froisser les maths.",
        "La corbeille vide, c’est Chuck Norris qui l’a déjà nettoyée.",
        "Chuck Norris met Windows à jour sans redémarrer.",
        "Le cloud sauvegarde Chuck Norris, pas l’inverse.",
        "Chuck Norris imprime en recto-verso… sur un post-it.",
        "Les commits de Chuck Norris n’ont pas d’historique. Ils sont éternels.",
        "Chuck Norris a déjà gagné une discussion avec un CAPTCHA.",
        "Le mode avion de Chuck Norris fait vraiment voler le téléphone.",
        "Chuck Norris a une boucle for qui finit avant de commencer.",
        "Quand Chuck Norris ping, Internet répond « présent ».",
        "Chuck Norris sait plier un papier plus de sept fois.",
        "Les fichiers de Chuck Norris font 0 octet mais contiennent tout.",
        "Chuck Norris a lu les conditions générales. Deux fois.",
        "Chuck Norris recharge une batterie avec un clin d’œil.",
        "Les secrets de Chuck Norris sont en clair et restent secrets.",
        "Chuck Norris a déjà mis une photo 3D sur un écran 2D.",
        "Les bugs signalent Chuck Norris, pas l’inverse.",
        "Chuck Norris éteint un feu… avec du piment.",
        "Chuck Norris compte jusqu’à l’infini. En nombres premiers.",
        "Le mode sombre éclaire quand Chuck Norris regarde.",
        "Chuck Norris souffle des bougies… par visioconférence.",
        "Le bluetooth de Chuck Norris s’appaire par télépathie.",
        "Chuck Norris lit les QR codes sans appareil photo.",
        "Les fuseaux horaires demandent l’autorisation à Chuck Norris.",
        "Chuck Norris défait un Rubik’s Cube sans le toucher.",
        "Les cookies demandent le consentement de Chuck Norris.",
        "Chuck Norris a déjà gagné à Pierre-Feuille-Ciseaux… en choisissant « Chuck ».",
        "Les API renvoient 200 rien que pour Chuck Norris.",
        "Chuck Norris met pause à un live.",
        "Le ventilateur refroidit quand Chuck Norris s’énerve.",
        "Chuck Norris sauvegarde en coupant le courant.",
        "Les to-do de Chuck Norris se cochent toutes seules.",
        "Chuck Norris coupe une corde… avec une capture d’écran.",
        "Le Wi-Fi capte mieux près de Chuck Norris.",
        "Chuck Norris compresse un fichier en le regardant.",
        "Les spoilers ont peur de Chuck Norris et restent muets.",
        "Chuck Norris mesure la vitesse de la lumière en pas.",
        "Les IA demandent des conseils à Chuck Norris.",
        "Chuck Norris ferme une application en soufflant sur l’icône."};
    std::string getRandomQuote()
    {
        size_t idx = std::rand() % CHUCK_FACTS.size();
        return (CHUCK_FACTS[idx]);
    }
};
#endif
