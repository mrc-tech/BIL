i file iniziano tutti con la stessa sequenza di 8 byte: 89 'P' 'N' 'G' 0D 0A 1A 0A


I dati sono inseriti in dei "chunk" che hanno un codice ASCII di 4 lettere all'inizio per distinguerli


mi sa che all'inizio di ogni scanline ci deve stare un byte che indica il filtro (esiste solo il filtro 00)
Quindi i dati dell'immagine 10x10 tutta rossa sono:
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 
0  ff 0 0  ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 ff 0 0 


L'algoritmo DEFLATE è quello usato da zlib, ma zlib se usato aggiunge due byte all'inizio (non so se è 78 5E) e 4 alla fine (checksum)
quello che c'è in mezzo sono dati da DEFLATE




