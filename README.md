# Bitmap

C++ library for images

This is a simple library mainly designed for image creation, but it can also read simple images format. With this tool you can create an image using several drawing functions:
 - set pixel color (with many palettes)
 - simple geometries (like lines, circles, bezier curves and more)
 - text in bitmap fonts

There are also some utility functions for upscaling, blurring and more.
For the moment the library can only write (and read) bitmap image formats, but the author is working to the png file format implementation. For write and read the images in the png format. When the library will be able to write a png image file the project will be renamed in `Simple Image Library (SIL)` and will be relased the first version, the `v1.0`.
The author is very busy with academic obligation so updates in this projct are not so often...

## Design objectives
 - All classes and functions in header files (no precompiled library or external dependencies)
 - Easy to use and the code can be understanded easly
 - Works on Windows, Linux and Android (C++11 standard)
 - Unlicensed (anyone can use and modify it as wanted)

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
}
```
It can be used also the single header-file `Image.h` which includes all the separated nested header files. This file is generated through the automated software [`header-merger`](https://github.com/mrc-tech/header-merger) developed by me.

For other, more complex, examples please see `examples` folder.

## Examples

![Mandelbrot_set](https://user-images.githubusercontent.com/74192712/128616712-8b2ed28f-9759-49a6-aa8d-c4bdda74c2e5.png)

![Lorentz_system](https://user-images.githubusercontent.com/74192712/128616684-a4f9e877-573b-46f6-ad36-f704f2cc23a7.png)

![Italy_EQ](https://github.com/mrc-tech/Bitmap/assets/74192712/977ff19a-567e-4bc6-a277-3b449da79913)




## ToDo
For the moment this _todo list_ is written in italian. As far as the first contributor comes I'll translate in English.

 - fare una classe "Figure" dove le coordinate sono double. La classe Image invece serve per le immagini raster. (non credo ce ne sia bisogno veramente...)
 - inserire anche una classe per gestire le immagini `PNG`. Dato che pesano meno dei bmp e vengono lette senza problemi da LaTeX (al contrario dei bitmap). Se inserisco il modo di gestire altri formati questo repository invece di "**Bitmap**" dovrebbe chiamarsi "**Image**"
 - fare l'esempio di _Perlin noise_ (studiare meglio come fare)
 - Se implemento i `PNG` e i `JPG` devo cambiare nome al repository in **Image**
 - Metodi per generare le figure geometriche fondamentali in `image` (mancano l'arco di cerchio, le splines [Per le splines usare il metodo di Cutmann, vedere come ho fatto in DOS] etc.)
 - inserire un metodo per il `flood-fill` delle figure (studiare bene come fare)
 - Sarebbe interessante anche un modo per "vettorializzare" i dati delle immagini in ingresso. Ma questo è di sicuro più complesso e richiederebbe proprio un altro repository ed un altro progetto.
 - Fare in modo che si possano anche _leggere_ i file PNG e JPG (almeno nella loro più semplice accezione)
 - fare in modo di poter scalare (anche di 2 e 0.5 soltanto) e di ruotare l'immagine (anche solo di +-90 gradi)
 - Chiamare questo progetto `SIL` (Simple Image Library)?? Anche `BIL` (Basic Image Library) è carino e anche simpatico
 - Fare una routine di *Anti Aliasing* sull'immagine
 - Fare un programma ulteriore (proprio un altro progetto) che trasforma la serie di immagini in un video... (oppure usare `ffmpeg`)
 - Mettere la funzione che disegna uno "squircle" (vedere il video su youtube). che gli dai _r_ e _n_ (capire come disegnare dalla formula). O anche solamente fare un esempio...
 - nell'introduzione di questo readme mettere un link di riferimento anche al futuro repo `HPlotter` che usa SIL per stampare i grafici in png
