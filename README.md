# Bitmap
C++ library for images

## ToDo
 - fare una classe "Figure" dove le coordinate sono double. La classe Image invece serve per le immagini raster.
 - inserire anche una classe per gestire le immagini `PNG`. Dato che pesano meno dei bmp e vengono lette senza problemi da LaTeX (al contrario dei bitmap). Se inserisco il modo di gestire altri formati questo repository invece di "**Bitmap**" dovrebbe chiamarsi "**Image**"
 - fare l'esempio di _Perlin noise_ (studiare meglio come fare)
 - fare esempi di generazione parametrica di figure (come i triangoli inscritti l'uno all'altro con una leggera rotazione)
 - Se implemento i `PNG` e i `JPG` devo cambiare nome al repository in **Image**
 - Metodi per generare le figure geometriche fondamentali in `image` (mancano l'arco di cerchio, l'ellisse, le splines, i poligoni regolari, etc.)
 - inserire un metodo per il `flood-fill` delle figure (studiare bene come fare)
 - Sarebbe interessante anche un modo per "vettorializzare" i dati delle immagini in ingresso. Ma questo è di sicuro più complesso e richiederebbe proprio un altro repository ed un altro progetto.
 - Fare in modo che si possano anche _leggere_ i file BMP, PNG e JPG (almeno nella loro più semplice accezione)
