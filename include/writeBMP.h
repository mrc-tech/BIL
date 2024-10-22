/**************************************************
writeBMP.h: scrive la matrice in un file bitmap
	la matrice contiene dati integer che descrivono
	l'intensit� di grigio da stampare.
	la matrice � salvata come un array monodimensionale
**************************************************/

#ifndef MRC_WRITE_BMP
#define MRC_WRITE_BMP


#include <fstream>
#include <string>
#include <vector>


void writeBMP(std::string filename,int rows,int cols,int R[],int G[], int B[])
{
	// i valori R G B sono interi (da 0 a 255)
	std::ofstream file(filename, std::ios::binary); // apre il file come binario
	int temp;
	
	//################################### FILE HEADER ###################################
	temp = 0x4D42;	file.write((char*)&temp,2); // intestazione "BM" (al contrario a causa del bigendian)
	temp = 54 + 3*rows*cols + (3*cols)%4; file.write((char*)&temp,4); // dimensione del file (54 bytes di header + dati RGB + zeri per raggiungere multipli di 4 byte)
	temp = 0;		file.write((char*)&temp,4); // spazio riservato
	temp = 54;		file.write((char*)&temp,4); // offset dell'header (da dove iniziano i dati)
	//################################### DIB HEADER ###################################
	temp = 40;		file.write((char*)&temp,4); // lunghezza in byte di questo header
	temp = cols;	file.write((char*)&temp,4); // Larghezza in pixel del bitmap
	temp = rows;	file.write((char*)&temp,4); // Altezza in pixel del bitmap
	temp = 1;		file.write((char*)&temp,2); // numero di piani (vale sempre 1)
	temp = 24;		file.write((char*)&temp,2); // numero di bit (colore) per pixel [24bit (3 x 8bit)]
	temp = 0;		file.write((char*)&temp,4); // compressione (nessuna compressione)
	temp = 3*rows*cols + (3*rows*cols)%4;	file.write((char*)&temp,4); // Dimensione in byte dell'immagine bitmap senza le 3 sezioni iniziali
	temp = 0x0B13;	file.write((char*)&temp,4); // Risoluzione orizzontale in px al mt (2.835 pixel/metro)
	temp = 0x0B13;	file.write((char*)&temp,4); // Risoluzione verticale in px al mt (2.835 pixel/metro)
	temp = 0;		file.write((char*)&temp,4); // Numero di colori da considerare nella palette (0 indica tutti)
	temp = 0;		file.write((char*)&temp,4); // zero indica che tutti i colori sono importanti
	
	//################################### DATI ###################################
	
	for(int r=rows-1;r>=0;r--){
		for(int c=0;c<cols;c++){
			temp = R[r*cols+c] % 256; file.write((char*)&temp,1); // scrive il contributo del rosso [1 byte]
			temp = G[r*cols+c] % 256; file.write((char*)&temp,1); // scrive il contributo del verde [1 byte]
			temp = B[r*cols+c] % 256; file.write((char*)&temp,1); // scrive il contributo del blu   [1 byte]
		}
		// completa la riga (deve essere un multiplo di 4 byte)
		if((3*cols)%4 != 0) for(int i=0;i<4-(3*cols)%4;i++){ // NON SO SE � GIUSTA LA MIA MODIFICA (ho bisogno di sapere quanto devo aggiungere per arrivare a 4, non il RESTO di quello che mi � rimasto....)
			temp = 0;
			file.write((char*)&temp,1);
		}
	}
	
	file.close();
}


// FUNZIONI ALTERNATIVE ==================================================================================================


void writeBMP(std::string filename,int rows,int cols,int mat[])
{
	// genera un'immagine tramite una matrice di interi da 0 a 255 che indicano il livello di grigio
	//   mat[i] = 0   : nero
	//   mat[i] = 255 : bianco
	writeBMP(filename, rows,cols, mat,mat,mat);
}


void writeBMP_hex(std::string filename,int rows,int cols,int mat[])
{
	// genera un'immagine tramite una matrice di interi da 0 a 255 che indicano il colore in esadecimale (24bit)
	int * R = new int[rows * cols];
	int * G = new int[rows * cols];
	int * B = new int[rows * cols];
	for(int r=0;r<rows;r++)
		for(int c=0;c<cols;c++){
			// BGR perch� � in Big Endian
			B[r*cols+c] = (mat[r*cols+c] & 0xFF0000) >> 16;
			G[r*cols+c] = (mat[r*cols+c] & 0x00FF00) >> 8;
			R[r*cols+c] = (mat[r*cols+c] & 0x0000FF);
		}
			
	writeBMP(filename, rows,cols, R,G,B);
}



#endif
