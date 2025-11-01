avrdude -v -c avrispv2 -p m32u2 -P COM4 -U "hfuse:w:0xd9:m"
avrdude -v -c avrispv2 -p m32u2 -P COM4 -U "lfuse:w:0xef:m"
avrdude -v -c avrispv2 -p m32u2 -P COM4 -U "flash:w:.\program\Ornament Argb\Ornament Argb\Release\Ornament Argb.hex"