edison_capture
-----------
Captures a still image from the lepton on the Intel Edison, using pins 10 - 13 for CS, MOSI, MISO and CLK. The image is stored as a PGM file under image.pgm.
Make sure, the MRAA library is up to date.

Compile with:
gcc -lmraa -o lepton_capture lepton_capture.c

Run with:
./lepton_capture