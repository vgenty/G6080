import sys, os
import subprocess32
from methods import *

def main():

    n_loops = 1000
    
    print "Calculating gcc -O3 and -O0 processes"
    
    data3 = []
    data0 = []

    for i in xrange(1000) :
        data0.append(subprocess32.check_output(["./real0","%d" % n_loops]).rstrip().split('\n'))
        data3.append(subprocess32.check_output(["./real3","%d" % n_loops]).rstrip().split('\n'))

        

    Plotter(data3,data0,int(sys.argv[1]),n_loops)        
    









if __name__ == '__main__':
    main()
