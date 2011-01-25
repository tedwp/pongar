#pragma once
class UI
{
public:
	UI(void);
	~UI(void);
	bool isMarkerVisible(int purpose);
	/*
	 * Greift auf Game.getMarkers() zu, um zu �berpr�fen, welche Marker gerade zu sehen sind.
	 * Jeder Marker sollte �ber einen lastVisible-timestamp verf�gen, so dass Marker, die seit mehr als z.B. drei Sekunden
	 * nicht mehr sichtbar waren, als unsichtbar gelten.
	 */
	void updateMarkerVisibilities(void);
};

