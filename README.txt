AVRAM Marius 334CB 
Tema 3 EGC 
Labyrinth 

Jocul a fost realizat pornit de la proiectul folosit in cadrul laboratorului 6
(cel cu camere FPS si TPS). Pe langa cele doua camere deja implementate in 
laborator a mai fost adaugat si o camera statica plasata undeva deasupra hartii.
De asemnea in modul third person personajul se misca o daca cu camera in orice 
directie (inainte, inaipoi, stanga, dreapta). Camera se roteste pe axa Oy cu 
ajutorul mouseului atat in modul first person cand si in cel third person. In 
cel din urma o data cu rotirea cameri pe axa Oy se poate observa si rotirea
personajului. Camera TPS poate face rotatii pe orice axa, axele Ox si Oz putand
fi controlate din taste (pt Ox tastele 't' si 'g', iar pentru Oz tastele 'q' si 
'e').
 
Din punct de vedere al implementarii camerelor am folosit doua clase. 
Cea deja existanta in scheletul de cod folosit in cadrul laboratorului pe care 
am redenumit-o DynamicCamera si inca o clasa pentru cea statica: Static Camera.
Am folosit doua pentru ca miscarea personajului este intotdeauna relativa la 
prima instanta a camerei si se doreste ca schimbarile pozitiei personajului sa 
fie vizibile si in modul static. La un moment dat este renderizata o singura
camera desi ambele au instante in clasa Game.

Labirintul este construit din cuburi. Acesta este citit din fisier. Exista 3 
harti si pot fi adaugate un numar nelimitat. De asemenea pozitiile personajului,
al portalului si al bonusurilor sunt genereate aleator la fiecare rulare.

Am implementat coliziuni cub-sfera pentru coliziunea personajului cu cuburile 
ce alcatuiesc peretii labirintului si coliziuni bounding-sphere cu obiectivele 
de pe harta (adica portal sau bonusuri).

Exista doua tipuri de bonusuri: unul care ofera transparenta peretilor pentru o 
perioada de timp si unul care iti permite sa distrugi peretii pentru o perioada 
de timp.

Cand se gaseste iesirea jucatorul primeste un mesaj de felicitare si de asemenea
i se ofera posibilitatea de a restarta jocul cu o noua harta si noi pozitii 
pentru jucator, portal si bonusuri. Dupa cum este cerut in enunt atunci cand 
atinge portalul jucatorul dispare.

Controalele jocului sunt urmatoarele:
miscare mouse - rotatie camera pe axa oy cu tot cu jucator
w,a,s,d - forward, left, back, right
q,e - rotatie in modul tps pe axa oz 
t,g - rotatie in modul tps pe axa ox
z,c - rotatie in modul tps pe axa oy fara rotatia jucatorului
r - resetare camera la starea initiala
click stanga mouse - distrugere blocuri (cand puterea de distrugere a blocurilor
este activa)
1 - camera FPS
2 - camera TPS
3 - camera statica

Alte detalii de implementare se gasesc in comentariile din cod.