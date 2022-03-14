#!/usr/bin/python3
import os

myenv = os.getenv("SHELL")
haha = "num1=1&num2=2"
haha.split("&");

print(myenv)
print(haha)

