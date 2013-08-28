@echo off
bmp32 merge diff32.bmp diff32a.bmp
del diffuse32.bmp
del .\default\diffuse32.bmp
ren out32.bmp diffuse32.bmp
copy diffuse32.bmp .\default
