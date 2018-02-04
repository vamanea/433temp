#!/usr/bin/env python2.7
import serial
import re
import requests

def basic_header():
	return {
		"Content-type": "text/plain"}

def put_status(name, teme):
    """ Put a status update to OpenHAB  key is item, value is state """
    url = 'http://openhab.lan:8081/rest/items/%s/state' % (name)
    req = requests.put(url, data=temp, headers=basic_header())
    if req.status_code != requests.codes.ok:
        req.raise_for_status()     

while True:
    try:
        with serial.Serial('/dev/ttyAMA0', 4800, timeout=80) as ser:
                while True:
                        line = ser.readline()   # read a '\n' terminated line
                        if line == []:
                            continue
                        result = re.match("Extracted temp: (\d+)", line)
                        if result:
                                temp = float(result.group(1))
                                temp /= 10
                                if temp > 40:
                                        continue
                                print(temp)
                                temp = "%.1f" % (temp)
                                put_status('Temperature_Balcony', temp)
                        print(line)
                ser.close()
    except:
        pass
    else:
        break
