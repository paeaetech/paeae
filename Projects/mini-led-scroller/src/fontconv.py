#!/usr/bin/env python2.6

import sys
import re

charlist = []
chardata = dict()

f = open(sys.argv[1])

lines = f.readlines()


def readcharline(l):
	b = 0
	i=0
	for c in l:
		if c == '|':
			break
		if c == 'X':
			b |= (1<<i)

		i+=1
	
	return b

lnum=0
def getline():
	global lnum
	if lnum < len(lines):
		d = lines[lnum]
		lnum+=1
		return d
	return None

l = getline()
while l:
	m = re.search('"([^"]+)"',l)
	if m:
		char = m.group(1)
		charlist.append(char)
		data = []
		
		while True:
			cl = getline()
			if not cl:
				lnum-=1
				break
				
			if cl.startswith('----'):
				lnum-=1
				break
			else:
				data.append(readcharline(cl))

		chardata[char]=data

	l = getline()
	
s="char s_charlist[] PROGMEM = {"
for c in charlist:
	if c in "'\\":
		s+="'\\"
	else:
		s+="'"
	s+="%s'," % c
	
s = s[:-1]+'};'
print s
print "char s_chardata[][5] PROGMEM = {"

for c in charlist:
	s = "\t{ "
	
	for x in range(5):
		l = 0
		
		for y in range(7):
			l |= ((chardata[c][y]>>x) & 1)<<y
			
		s+="0x%0x," % l
	
	s=s[:-1]+' '
	s+="},\t\t//'%s'" % c
	print s
	

print "};"