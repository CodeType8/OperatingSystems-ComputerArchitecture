#LDLIB=/usr/lib/gcc/arm-none-eabi/4.9.3/libgcc.a
LDLIB=/usr/lib/gcc/arm-none-eabi/6.3.1/libgcc.a

if [ $# \< 1 ]; then
   echo "mk FILENAME (without .c)"
   exit
fi

arm-none-eabi-as -mcpu=arm926ej-s us.s -o us.o
arm-none-eabi-gcc -c -w -mcpu=arm926ej-s -o $1.o $1.c #2> /dev/null
arm-none-eabi-ld -T u.ld us.o $1.o eoslib $LDLIB -Ttext=0x80000000 -o $1

sudo mount -o loop sdimage /mnt
sudo cp -av $1 /mnt/bin
#sudo ls -l /mnt/bin
sudo umount /mnt

rm $1 *.o

echo ready to go?
read dummy

qemu-system-arm -M versatilepb -m 256M -sd sdimage -kernel wanix \
-serial mon:stdio -serial /dev/pts/0






 




