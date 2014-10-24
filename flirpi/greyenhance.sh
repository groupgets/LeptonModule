./leptgraypng >x.png ; convert x.png -normalize -contrast -sharpen 5 -resize 320x240 y.png; fbi -T 1 -a y.png
