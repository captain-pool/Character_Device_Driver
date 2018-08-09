#! /usr/bin/python3
import os
v=os.system("make")
if not v:
    os.system("sudo rmmod chardev.ko")
    v=os.system("sudo insmod chardev.ko")
    if not v:
        v=os.system("sudo chmod 666 /dev/chrdev01")
        if v:
            print("\n ***Error chmod-ing")
            exit()
    else:
        print("\n ***Error insmod-ing")
        exit()
else:
    print("\n ***Error making")
    exit()
try:
    while 1:
        f=open("/dev/chrdev01","w")
        print(">>> ",end="")
        data=str(input())[:256]
        f.write(data)
        print("check dmesg")
        f.close()
except KeyboardInterrupt:
    print("\n")
    pass
except Exception as e:
        print(str(e))
