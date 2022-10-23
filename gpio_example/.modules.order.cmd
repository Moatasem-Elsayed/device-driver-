cmd_/home/pi/device_driver/gpio_example/modules.order := {   echo /home/pi/device_driver/gpio_example/hello.ko; :; } | awk '!x[$$0]++' - > /home/pi/device_driver/gpio_example/modules.order
