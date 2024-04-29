obj-m +=gpio_driver.o
obj-m +=led0.o
obj-m +=led1.o
obj-m +=led2.o
BDR=/lib/modules/6.5.0-28-generic/build


all:
	make -C ${BDR} M=${PWD} modules


