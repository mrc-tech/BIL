/*******************************************************
Simple Postprocessed Anti Aliasing (SPAA)
	praticamente una slinding window 3x3 che fa restitu-
	isce il colore del pixel in base ad una somma pesata
	dei pixel adiacenti. In alcuni casi (quelli che si 
	avvicinano alle linee perpendicolari) non esegue la 
	sfumatura data dalla somma pesata. 
	Ulteriori informazioni sono contenute nei fogli scr-
	itti a mano a San Benedetto nel 2021.

Copyright (c) 2021 Andrea Marchi
*******************************************************/




