KERN_DIR = /work/system/linux-3.4.2

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

#obj-m	+= at24cxx_dev.o
obj-m	+= at24cxx_drv.o
#obj-m	+= i2c_bus_s3c2440.o
