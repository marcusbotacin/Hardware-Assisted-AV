# -*- coding: utf8 -*-
# Hardware-Assisted AV
# Alternative implementation for taken branches extraction
# Marcus Botacin - 2017 - Federal University of Paraná (UFPR)

import sys          # Parameters/Arguments
import os           # Filesystem iteration
import fnmatch      # Filesystem filter
import subprocess   # Call PT client

# PT parser itself
class Parser():
    # class definition
    def __init__(self):
        # empty branch window:
        self.window = []
        # branch window size
        self.N = 20
        # PT client path
        self.PT="WindowsIntelPT\Compiled_IntelPt\\"
        # PT client option
        self.PT_opt="N"
        # PT client core
        self.PT_core="1"

    # run PT client
    def run(self,app_name):
        # cmd line to call PT
        proc_cmd=self.PT+"PtControlApp.exe"
        p = subprocess.Popen(proc_cmd, stdout=subprocess.PIPE, stdin=subprocess.PIPE, stderr=subprocess.PIPE)
        # stdin to PT client
        proc_args=self.PT_opt+"\n"+app_name+"\n"+self.PT_core+"\n"
        p.communicate(input=proc_args)

    # get PT log file
    def get_log(self):
        # regex into output directory
        dumps = fnmatch.filter(os.listdir(self.PT), '*Dumps')
        # return most recent occurrence
        return dumps[-1]

    # adjust window to fit given size
    def adjust_window(self,taken):
        # always add to the window
        self.window.append(taken)
        # size exceeded
        if(len(self.window)>self.N):
            # cut to fit
            self.window = self.window[1:]
        # print window, for debug
        if(len(self.window)==self.N):
            print(''.join(self.window))

    # extract windows from log file
    def handle_log(self,log):
        # log file
        cmd = self.PT+"\\"+log+"\cpu00_text.log"
        # open log file
        f = open(cmd)
        # for each line in file
        for line in f:
            # for branch data
            if "tnt" in line:
                # extract individual branches
                branches = line.strip().split("tnt.8")[1].strip()
                # for each branch
                for branch in branches:
                    # taken
                    if "!" in branch:
                        taken=str(1)
                    # not taken
                    else:
                        taken=str(0)
                    # add to window
                    self.adjust_window(taken)

# Usage warning
def usage(bin_name):
    print("Python %s <app_name>" % bin_name)

# parse arguments
def parse(args):
    # arguments supplied
    if len(args) > 1:
        # consider the first as application name
        return args[1]
    # no args
    # display usage
    usage(args[0])
    # go away
    sys.exit(0)

# "Main"
if __name__ == '__main__':
    app_name = parse(sys.argv)
    p = Parser()
    p.run(app_name)
    log = p.get_log()
    p.handle_log(log)
# no module support
else:
    print("No module import support")
