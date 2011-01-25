#pragma once
class UI
{
public:
	UI(void);
	~UI(void);
	bool isMarkerVisible(int purpose);
	/*
	 * Greift auf Game.getMarkers() zu, um zu überprüfen, welche Marker gerade zu sehen sind.
	 * Jeder Marker sollte über einen lastVisible-timestamp verfügen, so dass Marker, die seit mehr als z.B. drei Sekunden
	 * nicht mehr sichtbar waren, als unsichtbar gelten.
	 */
	void updateMarkerVisibilities(void);
};

