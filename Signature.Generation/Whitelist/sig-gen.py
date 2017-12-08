# Hardware-Assisted AV
# Signature Generation
# Whitelist component
# Marcus Botacin - 2017
# Federal University of Paraná (UFPR)

import sys  # args
import os   # filesystem

# program banner
head="SIG-GEN"
# signature size
N = 20

# display usage message
def usage(bin_name):
	print("python %s -m <malware dir> -g <goodware dir>" % bin_name)
	print("python %s -m <malware dir> -g <goodware dir> -n <window size>" % bin_name)

# return a list of malware files from the malware directory
def get_malware_files(mw_dir):
	files=[]
	for file in os.listdir(mw_dir):
		files.append(file)
	return mw_dir+'/'+files[0],mw_dir+'/'+files[1]

# the same for goodware files
def get_goodware_files(gw_dir):
	files = []
	for f in os.listdir(gw_dir):
		files.append(gw_dir+'/'+f)
	return files

# branch pattern from file
def build_window_from_file(_file):
	windows = set()
	current_window = []
	f = open(_file,"r").read().strip()
        # for each branch in file
	for branch in f:
                # add to the current window
		current_window.append(branch)
                # size is ok
		if len(current_window) == N:
                        # generate tuple
			windows.add(tuple(current_window))
                        # cut to next round
			current_window=current_window[1:N]
	return windows

# build pattern list from each goodware file in the list
def build_goodware_window(gw_files):
	windows = set()
	for _file in gw_files:
		w = build_window_from_file(_file)
		windows=windows.union(w)
	return windows

# check if two patterns are the same
def compare_windows(w1,w2):
	diff = set(w1-w2)
	return diff

# program main
def main(mw_dir,gw_dir):

        # banner
	print("[%s] Starting Signature Generation" % head)

        # debug print
	if __debug__:
		print("[%s] Malware dir is: %s" % (head,mw_dir))
		print("[%s] Goodware dir is: %s" % (head,gw_dir))

        # get list of malware trace files
	mw_lib, mw_text = get_malware_files(mw_dir)

        # debug print
	if __debug__:
		print("[%s] Malware lib file is: %s" % (head,mw_lib))
		print("[%s] Malware text file is: %s" % (head,mw_text))

        # get list of goodware files
	gw_files = get_goodware_files(gw_dir)

        # debug print
	if __debug__:
		print("[%s] Goodware file are: %s" % (head,gw_files))

        # build windows from goodware files (including libs)
	mw_lib_windows = build_window_from_file(mw_lib)
	if __debug__:
		print("[%s] Malware lib windows: %d" % (head,len(mw_lib_windows)))

        # build windows from malware files (excluding libs)
	mw_text_windows = build_window_from_file(mw_text)
	if __debug__:
		print("[%s] Malware text windows: %d" % (head,len(mw_text_windows)))

        # build windows from goodware files (excluding libs)
	gw_windows = build_goodware_window(gw_files)
	if __debug__:
		print("[%s] Goodware windows: %d" % (head,len(gw_windows)))

        # candidate windows I (malware not in goodware)
	candidate_windows = compare_windows(mw_text_windows,gw_windows)
	if __debug__:
		print("[%s] Malware win not in goodware win: %d" % (head,len(candidate_windows)))

        # candidate windows II (malware not in libs)
	text_candidate_windows = compare_windows(candidate_windows,mw_lib_windows)
	if __debug__:
		print("[%s] Malware win not in malware lib win: %d" % (head,len(text_candidate_windows)))

        # display signatures
	print("[%s] There are %d candidate windows for signature generation" % (head,len(text_candidate_windows)))
	for w in text_candidate_windows:
		print("[%s] Signature candidate: %s" % (head,''.join(w)))

# parse arguments
def parse(args):
	global N
	mw_dir = None
	gw_dir = None

        # incorrect args
	if(len(args)!=5 and len(args)!=7):
                # display usage
		usage(args[0])
                # go away
		sys.exit(0)

        # malware directory
	if args[1] == "-m":
		mw_dir = args[2]
        # goodware directory
	if args[3] == "-g":
		gw_dir = args[4]

        # optional args
	if len(args)==7:
                # redefining windows size
		if args[5] == '-n':
			N = int(args[6])

        # if having args but incorrect ones
	if mw_dir is None or gw_dir is None:
                # display usage
		usage(args[0])
                # go away
		sys.exit(0)

        # success
	return mw_dir,gw_dir

# program entry
if __name__ == '__main__':
        # parse args
        # get malware and goodware dirs
	mw_dir,gw_dir = parse(sys.argv)
        # then call main
	main(mw_dir,gw_dir)
else:
        # no class, no module import
	print("No module import support yet!")
