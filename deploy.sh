sudo chmod 666 /dev/ttyUSB0
make boot.scr 1> /dev/null
echo "C-a C-s"
echo "choose file: ev3ninja.bin"
cat boot.cmd

picocom -b 115200 --send-cmd="sb -vv" /dev/ttyUSB0
