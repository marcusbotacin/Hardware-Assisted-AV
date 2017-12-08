# Hardware-Assisted AV
# Signature Generation
# Neural Network component
# Marcus Botacin - 2017
# Federal University of Parana (UFPR)

import pickle                                       # dump/load model
import sys                                          # args

# patterns
good_sigs=set()
bad_sigs=set()
# signature size
N=20

# get pattern from file
# as in train.py
def get_window(_file):
	tmp_set=set()
	window=[]
	f=open(_file,"r").read().strip().split("\n")
	for branch in f[0]:
		window.append(int(branch))
		if(len(window)==N):
			tmp_set.add(tuple(window))
			window=window[1:]
	return tmp_set

# load ML model
clf = pickle.load(open("model", 'rb'))

#  Test

# get windows from specified file
x = get_window(sys.argv[1])

# count good and bad rates
counter = dict()
counter[0]=0
counter[1]=0

# as well as count repeated pattern ratio
not_uniq = 0
uniq=0

# for each pattern
for sig in x:
        # predict bad/good
	z=clf.predict([sig])
        # count occurence
	counter[z[0]]+=1
        # count unique or repeated
	if(z[0]):
		if sig not in good_sigs:
			uniq+=1
		else:
			not_uniq+=1

# print stats
print("0: %d\n1: %d\nR: %f" % (counter[0],counter[1],100*counter[1]/float(counter[0])))
print("Uniques: %d\nNot Uniques: %d" % (uniq,not_uniq))
