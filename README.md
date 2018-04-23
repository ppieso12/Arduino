# Arduino tm1638 based board project

https://drive.google.com/open?id=0B3Xrl23cTkWuYjJOY2NOWHpXUU0

Program  ma  imitować  proces  łamania  kodu  zadanego  w  postaci  ciągu  ośmiu  cyfr 
szesnastkowych (‘0’..’9’,’A’,’b’,’C’,’d’,’E’,’F’). Na wszystkich segmentach wyświetlane są szybko 
zmieniające się kolejne cyfry szesnastkowe. Po  n-krotnym wyświetleniu na skrajnie lewym 
segmencie  cyfry  odpowiadającej  pierwszemu  znakowi  kodu,  cyfra  ta  jest  wyświetlana  w 
sposób  ciągły  (kod  na  pozycji  nr  1  został  złamany),  a  proces  łamania  dotyczy  kolejnych 
segmentów (od lewej do prawej strony). Złamanie każdego znaku powinno być sygnalizowane 
włączenie odpowiedniej diody LED (LED1..LED8). Po złamaniu całego kodu (ośmiu znaków)   
program oczekuje na wprowadzenie (poprzez monitor portu szeregowego) nowego kodu lub 
na naciśnięcie przycisku S2 – wtedy ponownie łamany jest dotychczasowy kod. Naciśnięcie w 
trakcie  procesu  łamania  kodu  przycisku  S1  powoduje  wstrzymanie  łamania,  a  kolejne 
naciśnięcie  wznawia  łamanie  –  naciskanie  przycisku  S2  w  trakcie  łamania  nie  powinna 
powodować żadnej akcji.  
Program powinien akceptować następujące komendy (wprowadzane z PC poprzez monitor 
łącza szeregowego): 
* Cxxxxxxxx  - kod w postaci ciągu ośmiu znaków szesnastkowych 
* Nxx    - liczba powtórzeń (1..99) wyświetleń określająca złamanie znaku 
* Dxxxx    - czas wyświetlania pojedynczego znaku w trakcie łamania (w ms, zakres 1..9999), pozwalający na regulację prędkości łamania kodu 
Program  powinien  sygnalizować  (poprzez  wyświetlenie  odpowiedniego  komunikatu) 
ewentualne błędy w podanych komendach lub parametrach. 
Po  starcie  program  powinien  wykorzystać  wartości  domyślne  zapisane  w  sposób 
umożliwiający ustalenie ich wartości poprzez łatwą modyfikację kodu źródłowego na etapie 
demonstracji działania programu (edycja przed wgraniem kodu do mikrokontrolera). 
Użycie  komend  w trakcie działania  programu powinno  skutkować natychmiastową  zmianą 
odpowiednich parametrów. 
Parametry (ustalane przed kompilacją kodu): 
CODE  - ciąg ośmiu cyfr szesnastkowych stanowiących kod 
TIMES - liczba powtórzeń wyświetleń określająca złamanie znaku 
DISP  - czas wyświetlania pojedynczego znaku 

https://blog.3d-logic.com/2015/01/10/using-a-tm1638-based-board-with-arduino/
