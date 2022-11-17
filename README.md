# Duck Ninja

## Purpose of project

Celem projektu jest stworzenie gry o nazwie “Duck Ninja”, w której użytkownik będziezbierał punkty przez przecinanie przeciwników - kaczek-zombie mieczem.

## System setup

To run example OpenGL 4.2 is needed (build in on MacOS) and GLFW.
To install GLFW (MacOS):

```bash
  brew install glfw
```

Also on M1 you need to export path to homebrew installation directory.
See [https://stackoverflow.com/questions/67373307/macos-m1-fatal-error-glfw-glfw3-h-file-not-found](here).

## TODO

- [ ] Ładowanie obiektów z pliku
- [ ] Generowanie terenu, stworzenie podstawowej sceny
- [ ] Nawigacja po scenie za pomocą myszki i klawiatury
- [ ] Umożliwienie podglądu sceny w trybie wireframe
- [ ] Ładowanie zewnętrznych, teksturowanych obiektów
- [ ] Stworzenie shaderów pokazujących obrażenia zadawane postaci przez wrogów
- [ ] Animacje ruchu przeciwników
- [ ] Implementacja fizyki członków przeciwników
- [ ] Oświetlenie zgodne z modelem Phonga, punktowe źródła światła, zastosowanie dynamicznego oświetlania, bez uwzględnienia cieni
