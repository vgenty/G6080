import numpy as np

class Planet:
    def __init__(self,nsteps,NP):
        
        self.nsteps = nsteps
        self.NP     = NP

        self.dx = pow(2,-4.0)
    
        self.r  = np.zeros((nsteps,NP,3),dtype=float)    # initial position
        self.v  = np.zeros((nsteps,NP,3),dtype=float)    # initial velocity
        
        self.m  = np.array([1.00,
                            0.01,
                            0.01])
        
        self.ke = np.zeros((NP,nsteps),dtype=float)
        self.pe = np.zeros((NP,nsteps),dtype=float)
        
        self.KEtot = np.zeros(nsteps,dtype=float)
        self.PEtot = np.zeros(nsteps,dtype=float)
        self.Etot  = np.zeros(nsteps,dtype=float)
        
        self.fr  = np.zeros((nsteps,NP,3),dtype=float)    # forces R
        self.fv  = np.zeros((nsteps,NP,3),dtype=float)    # forces V
    
        
        #initialize variables

        self.r[0][1][0] =  1.00
        self.r[0][2][0] = -1.00
        
        self.v[0][1][1] =  1.00
        self.v[0][2][1] =  1.04        

        for k in xrange(3):
            self.F(0,k)
            
        self.ff = 0.0;
        self.denom = 0.0;

    def __str__(self):
        return "Planet struct (class)"        
    
        
    def KE(self):
        return float(0.5 * self.m * ( self.v[0]**2 + 
                                      self.v[1]**2 + 
                                      self.v[2]**2 ) )            
        
    def evolve(self):
        for i in xrange(self.nsteps-1):
            for j in xrange(self.NP):
                #will clean up
                for k in xrange(3):
                    self.r[i+1][j][k] = self.r[i][j][k] + self.dx * self.fr[i][j][k]
                    self.v[i+1][j][k] = self.v[i][j][k] + self.dx * self.fv[i][j][k]

            for j in xrange(self.NP):
                for k in xrange(3):
                    self.F(i+1,k)

            for j in xrange(self.NP):
                for k in xrange(3):
                    self.r[i+1][j][k] = self.r[i][j][k] + self.dx/2.0 * (self.fr[i][j][k]
                                                                         + self.fr[i+1][j][k])
                    self.v[i+1][j][k] = self.v[i][j][k] + self.dx/2.0 * (self.fv[i][j][k]
                                                                         + self.fv[i+1][j][k])

            for j in xrange(self.NP):
                for k in xrange(3):
                    self.F(i+1,k)


    def F(self,i,k):
        for j in xrange(self.NP): # planet number
            #for k in xrange(3):   # x,y,z
            self.fr[i][j][k] = self.v[i][j][k] 
            self.fv[i][j][k] = self.force(i,j,k) #calculate force on j'th planet along k direction
        
    def force(self,i,j,k):
        #print "@@@@@@@@@@@@@@@@ i = %d @@@@@@@@@@@@@" % i
        #print self.r[i][j]
        self.ff = 0.0            # force on planet j
        for l in xrange(self.NP): # planet number l
            self.denom = 0.0
            if j is not l:
                for b in xrange(3):
                    #print "Planet %d at %0.15f on %d %0.15f, coordinate %d" % (j,self.r[i][j][b],l,self.r[i][l][b],b)
                    #print "self.denom = %0.15f" % self.denom
                    self.denom += pow(self.r[i][l][b] - self.r[i][j][b],2.0)
                    
                #print "self.ff = %0.15f" % self.ff
                self.ff += (self.m[l] * (self.r[i][j][k] - self.r[i][l][k]) / pow(self.denom,1.5))
                #print "diff: %0.15f: %f - %f " % (self.r[i][j][k],self.r[i][l][k],(self.r[i][j][k] - self.r[i][l][k]))
                #print "self.ff = %0.15f" % self.ff


        return -1.0*self.ff
