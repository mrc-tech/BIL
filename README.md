# Bitmap
C++ library for images

## Usage
To use the class include the "`Image.hpp`" header file (inside the `include` folder, other header files in this folder are necessary):
```c++
#include "Image.hpp"          // include the Image class

int main()
{
   Image img(800,600);        // create an 800x600 pixels image
   img.clear(255,255,255);    // set all pixels to white
   img.line(0,0,800,800);     // draw a diagonal line on the image
   
   img.save_bmp("image.bmp"); // save image as 24-bit Bitmap file
   
   return 0;
}
```
For other, more complex, examples please see `examples` folder.


## ToDo
 - fare una classe "Figure" dove le coordinate sono double. La classe Image invece serve per le immagini raster. (non credo ce ne sia bisogno veramente...)
 - inserire anche una classe per gestire le immagini `PNG`. Dato che pesano meno dei bmp e vengono lette senza problemi da LaTeX (al contrario dei bitmap). Se inserisco il modo di gestire altri formati questo repository invece di "**Bitmap**" dovrebbe chiamarsi "**Image**"
 - fare l'esempio di _Perlin noise_ (studiare meglio come fare)
 - Se implemento i `PNG` e i `JPG` devo cambiare nome al repository in **Image**
 - Metodi per generare le figure geometriche fondamentali in `image` (mancano l'arco di cerchio, le splines etc.)
 - inserire un metodo per il `flood-fill` delle figure (studiare bene come fare)
 - Sarebbe interessante anche un modo per "vettorializzare" i dati delle immagini in ingresso. Ma questo è di sicuro più complesso e richiederebbe proprio un altro repository ed un altro progetto.
 - Fare in modo che si possano anche _leggere_ i file PNG e JPG (almeno nella loro più semplice accezione)
 - fare in modo di poter scalare (anche di 2 e 0.5 soltanto) e di ruotare l'immagine (anche solo di +-90 gradi)
 - Chiamare questo progetto `CIL` (C++ Image Library)??
 - Forse dovrei mettere delle immagini (tra le top 3 piu belle e rappresentative tra gli esempi) qua nel README.md principale, nella categoria "Examples". In maniera tale che chi visita il mio repo vede in anteprima cosa puo fare con la mia libreria... (forse dovrei aspettare di avere un modo di esportare le immagini in formato `png`, in maniera tale da non appesantire il sito)
