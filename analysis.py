#!/usr/bin/python
import numpy as np
import csv as csv
import math
import sys


def mean(numbers):
	return float(sum(numbers)) / max(len(numbers), 1)
def math_round(number):
	c = math.ceil(number)
	f = math.floor(number)
	if c - number > number - f:
		return f
	else:
		return c

def read_file(fname):
	#readdata = csv.reader(open('temp_25_6_ch0.csv', 'r'))
	readdata = csv.reader(open(fname, 'r'))

	data = []

	for row in readdata:
	    try:
		data.append([float(row[0]) * 100, int(row[1].strip())])
	    except:
		pass

	#find start
	for index, row in enumerate(data):
		if row[1] == 1:
		    break

	highs = []
	for i in range(index, len(data)):
		if data[i][1] == 0: continue
		highs.append(data[i+1][0] - data[i][0])
	#	print i

	pulse = mean(highs)
	#print (("Mean high interval: %fms") % pulse)

	lows = []
	for i in range(index + 1, len(data)):
		if data[i][1] == 1: continue
		if i + 1 >= len(data): break
		#blen = (data[i+1][0] - data[i][0]) / pulse
		alen = math_round((data[i+1][0] - data[i][0]) / pulse)
		#print("Truncate %.02f to %u" % (blen, alen)) 
		if alen == 2:
		    lows.append(str(1))
		else:
		    lows.append(str(0))
	lows.append(str(0))
	#first = "".join(lows[:])
	#print "%s" % (first)
	for i in range(0, 3):
		first = "".join(lows[i * 46: (i + 1) * 46])
		print "%s" % (first)
		print "%s %s" % (first[0:25], first[25:])
		seq = list(first[25:])
		print "%s, %s, %s" % (fname, first[25:], ",".join(seq))

		#last = "".join(reversed(first[25:]))
		#print last[-10:]
		#print hex(int(last[-10:]))
		#print hex(int(first,2))
		break


args = sys.argv[1:]
while len(args):
    #print args[0]
    read_file(args[0])
    args = args[1:] # shift
