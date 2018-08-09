## First Linux Character Device Driver
### With Mutex Lock
Read/Write into Pseudo Character Device Driver.
to check the output of write run:
```sh
    sudo dmesg
```
or keep this running on another terminal
```sh
tail -f cat /var/log/kern.log
```
To run the module,
```sh
    python3 test.py
```
This will make/change permissions/open a terminal to send data to device
to read the data
```sh
    cat /dev/chardev01
```
