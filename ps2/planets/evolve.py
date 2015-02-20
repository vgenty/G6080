#!/usr/bin/python
import numpy as np

import planet

def main():
    nsteps = 30000
    NP  = 3                # num planets

    p = planet.Planet(nsteps,NP)

    p.evolve()
    
if __name__ == '__main__':
    main()
