# Vorgehen #
  * Drei Marker Erkennen
  * Feld dem Marker zugeordnet einblenden
  * Feldposition merken, Marker ab jetzt ignorieren
  * Feldkoordinatensystem berechnen, geeignete fixe Feldgröße (z.B. 800 `*` 600 bzw. 4:3 beim Original)
  * Paddles im Feld beweglich machen
  * Ball und Kollision implementieren (Animation unabhängig von CPU und Aktualisierungsgeschwindigkeit halten, z.B. mittels verschieden oft aufgerufener ball->refresh() - Methode)
  * Kollision bei nicht vom Paddle belegten linken und rechten Randflächen deaktivieren
  * Punktezählung
  * Fertig