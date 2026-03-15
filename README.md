# ❌ Tic-Tac-Toe Collection (C & SDL2) ⭕

O colecție de implementări ale jocului **Tic-Tac-Toe** în limbajul C, variind de la o versiune avansată cu interfață grafică (SDL2) până la o variantă de consolă cu AI imbatabil.

---

## 🛠️ Versiuni Incluse

### 1. Interfața Grafică (SDL2)
Fișier principal: `tictactoe_sdl.c` (sau versiunea `_fixed.c`)

* **Grafică Dinamică:** Simboluri X și O desenate prin algoritmi matematici, cu efecte de animație la plasare.
* **Audio:** Suport pentru efecte sonore la fiecare mutare și la câștigarea jocului.
* **Sistem de Scor:** Salvează automat progresul jucătorilor în `scor.txt`.
* **UI Complet:** Include buton de **Restart**, mesaje de stare (ex: "Randul jucatorului X") și afișarea scorului în timp real.

### 2. Versiunea de Consolă (AI & Logică)
Fișier principal: `X&0.c`

* **Algoritm Minimax:** Include un AI inteligent care evaluează toate mutările posibile pentru a asigura un joc perfect la dificultate maximă.
* **3 Niveluri de Dificultate:** * *Ușor:* Mutări aleatorii.
    * *Mediu:* Mix între logică și hazard.
    * *Greu:* AI imbatabil folosind Minimax.
* **Vizual:** Folosește culori ANSI în terminal (Roșu pentru X, Albastru pentru O, Verde pentru linia câștigătoare).

---

## 🚀 Compilare și Rulare

### Dependențe necesare
Asigură-te că ai instalate bibliotecile de dezvoltare SDL2:
* `SDL2`
* `SDL2_ttf`

### Comenzi Consolă (Linux/macOS/MinGW)

**Pentru versiunea Grafică (GUI):**
```bash
gcc tictactoe_sdl.c -o tictactoe_gui -lSDL2 -lSDL2_ttf -lm
./tictactoe_gui

** Compilare versiunea SDL2:**
```bash
gcc tictactoe_sdl.c -o tictactoe_gui -lSDL2 -lSDL2_ttf -lm
./tictactoe_gui

---
