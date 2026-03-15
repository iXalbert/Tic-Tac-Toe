#❌ Tic-Tac-Toe Collection (C & SDL2) ⭕
##Acest repository conține două implementări distincte ale jocului Tic-Tac-Toe (X și Zero), dezvoltate în C. Proiectul explorează atât programarea grafică folosind biblioteca SDL2, cât și implementarea algoritmilor de inteligență artificială (Minimax) pentru gameplay-ul în consolă.

#🛠️ Versiuni Disponibile
##1. Interfață Grafică (SDL2)

Implementată în tictactoe_sdl.c, această versiune oferă o experiență vizuală modernă.

Grafică & Animații: Simboluri desenate dinamic cu efecte de scalare.

Audio: Efecte sonore pentru mutări și victorie (move.wav, win.wav).

Sistem de Scor: Salvarea și încărcarea automată a scorului dintr-un fișier text (scor.txt).

UI: Buton de restart funcțional și afișarea mesajelor de stare prin SDL_ttf.

##2. Versiunea de Consolă (CLI + AI)

Implementată în X&0.c, axată pe logică și algoritmi.

AI Inteligent: Trei niveluri de dificultate:

Ușor: Mutări aleatorii.

Mediu: Combinație între joc inteligent și aleator.

Greu: Algoritmul Minimax (nu poți câștiga împotriva lui).

Colorare Terminal: Utilizarea secvențelor ANSI pentru a evidenția X (roșu), O (albastru) și linia câștigătoare (verde).

#🚀 Instalare și Cerințe
##Dependențe (pentru versiunea grafică):

Pentru a compila versiunea SDL2, vei avea nevoie de:

SDL2

SDL2_ttf

SDL2_mixer (pentru suport audio)

Compilare:

Pentru versiunea SDL2:

Bash
gcc tictactoe_sdl.c -o tictactoe_gui -lSDL2 -lSDL2_ttf -lm
Pentru versiunea Consolă:

Bash
gcc "X&0.c" -o tictactoe_cli
#🎮 Cum se rulează?
GUI: Asigură-te că ai fișierele arial.ttf, move.wav și win.wav în același folder cu executabilul.

CLI: Pur și simplu rulează ./tictactoe_cli și alege modul de joc (vs AI sau vs Prieten) direct din meniu.

#🧪 Testare
Codul include unit teste bazate pe assert() pentru a verifica corectitudinea logicii de joc:

Validarea corectă a liniilor câștigătoare (orizontal, vertical, diagonal).

Detectarea stării de remiză.
