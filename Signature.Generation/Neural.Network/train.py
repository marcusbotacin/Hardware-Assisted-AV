# Hardware-Assisted AV
# Signature Generation
# Neural Network component
# Marcus Botacin - 2017
# Federal University of Parana (UFPR)

from sklearn.neural_network import MLPClassifier    # MLP
import os                                           # read files
import pickle                                       # dump/load

# branch patterns
good_sigs=set()
bad_sigs=set()
# pattern size
N=20

# generate patterns from file
def get_window(_file):
	tmp_set=set()
	window=[]
        # open file
	f=open(_file,"r").read().strip().split("\n")
        # for each branch in file
	for branch in f[0]:
                # try to add to the window
		window.append(int(branch))
                # if fits size, generate tuple
		if(len(window)==N):
                        # add tuple to database (set)
			tmp_set.add(tuple(window))
                        # cut window to next round
			window=window[1:]
        # return tuples set
	return tmp_set

# for good files
for good in os.listdir("good"):
        # get patterns for this file
	x=get_window("good/"+good)
        # merge with existing patterns
	good_sigs=good_sigs.union(x)

# for bad files
for bad in os.listdir("bad"):
        # get patterns for this file
	x=get_window("bad/"+bad)
        # merge with other files
	bad_sigs=bad_sigs.union(x)

# ML itself

X=[]    # objects
Y=[]    # labels

# for good
for tup in good_sigs:
        # list of all objects
	X.append(list(tup))
        # all labels are 0 (good)
	Y.append(0)

# for bad
for tup in bad_sigs:
        # all objects
	X.append(list(tup))
        # all labels are 1 (bad)
	Y.append(1)

# Train

clf = MLPClassifier(solver='lbfgs', alpha=1e-5, hidden_layer_sizes=(11), random_state=1)
clf.fit(X, Y)

# Dump model

pickle.dump(clf, open("model", 'wb'))
