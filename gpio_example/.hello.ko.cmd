cmd_/home/pi/device_driver/gpio_example/hello.ko := ld -r -EL --build-id=sha1  -T scripts/module.lds -o /home/pi/device_driver/gpio_example/hello.ko /home/pi/device_driver/gpio_example/hello.o /home/pi/device_driver/gpio_example/hello.mod.o;  true