import zlib

s = '78 5e 63 f8 cf 80 0f 8d 4a 23 85 0f 00 df 60 60 a0' # prova.png
# s = '08D763F8CFC0000003010100' # hardcoded red single pixel
b = bytearray.fromhex(s)
compress = zlib.compressobj(zlib.Z_DEFAULT_COMPRESSION, zlib.DEFLATED, -15)
z = zlib.decompress(b)
print(''.join('{:02x} '.format(x) for x in z))
print('')



s = 'pollo'
b = s.encode() # converte in binario
z = zlib.compress(b, -1) # comprime
print(''.join('{:02x} '.format(x) for x in z)) # stampa i dati compressi in esadecimale

