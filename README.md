# Bitmap
C++ library for images

## Usage
To use the class include the "`Image.hpp`" header file (inside the `include` folder, other header files in this folder are necessary):
```c++
#include "Image.hpp"          // include the Image class

int main()
{
   Image img(800,600);        // create an 800x600 pixels image
   img.clear();               // set all pixels to black
   
   img.save_bmp("image.bmp"); // save image as 24-bit Bitmap file
   
   return 0;
}
```
For other, more complex, examples please see `examples` folder.


## ToDo
 - fare una classe "Figure" dove le coordinate sono double. La classe Image invece serve per le immagini raster.
 - inserire anche una classe per gestire le immagini `PNG`. Dato che pesano meno dei bmp e vengono lette senza problemi da LaTeX (al contrario dei bitmap). Se inserisco il modo di gestire altri formati questo repository invece di "**Bitmap**" dovrebbe chiamarsi "**Image**"
 - fare l'esempio di _Perlin noise_ (studiare meglio come fare)
 - fare esempi di generazione parametrica di figure (come i triangoli inscritti l'uno all'altro con una leggera rotazione)
 - Se implemento i `PNG` e i `JPG` devo cambiare nome al repository in **Image**
 - Metodi per generare le figure geometriche fondamentali in `image` (mancano l'arco di cerchio, l'ellisse, le splines, i poligoni regolari, quadrilateri [quad] etc.)
 - inserire un metodo per il `flood-fill` delle figure (studiare bene come fare)
 - Sarebbe interessante anche un modo per "vettorializzare" i dati delle immagini in ingresso. Ma questo è di sicuro più complesso e richiederebbe proprio un altro repository ed un altro progetto.
 - Fare in modo che si possano anche _leggere_ i file BMP, PNG e JPG (almeno nella loro più semplice accezione)
 - Fare un esempio dell'*attrattore strano di Lorentz* e mettere anche le scritte con le formule e i rispettivi parametri usati
