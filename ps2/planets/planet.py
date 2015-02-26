import numpy as np

class Planet:
    def __init__(self,nsteps,NP):
        
        self.nsteps = nsteps
        self.NP     = NP

        self.dx = 1.0*np.power(10,-5.0)
    
        self.r  = np.zeros((nsteps,NP,3),dtype=float)    # initial position
        self.v  = np.zeros((nsteps,NP,3),dtype=float)    # initial velocity
        
        self.m  = np.array([1.00,
                            0.01,
                            0.005,
                            0.00001])
        
        self.KEtot = np.zeros(nsteps,dtype=float)
        self.PEtot = np.zeros(nsteps,dtype=float)
        self.Etot  = np.zeros(nsteps,dtype=float)
        
        self.fr  = np.zeros((nsteps,NP,3),dtype=float)    # forces R
        self.fv  = np.zeros((nsteps,NP,3),dtype=float)    # forces V
        
        # initialize variables position and velocity

        self.r[0][1][0] =  1.00
        self.r[0][2][0] =  2.0/3.0
        self.r[0][3][0] =  0.99
        
        self.v[0][1][1] =  1.00
        self.v[0][2][1] =  np.power((2.0/3.0),-0.5)
        self.v[0][3][0] =  -8.0   # x velocity
        self.v[0][3][1] =   1.225 # y velocity

        # misc variables        
        self.xyz = xrange(3)
        self.ff = 0.0;
        self.denom = 0.0;
        
        # initialize Forces, KE and PE
        
        for k in self.xyz:
            self.F(0,k,1)
            
        self.K(0)
        self.E(0)

        #self.PE(0)
            
    def __str__(self):
        return "Planet struct (class)"        

    def E(self,i):
        self.Etot[i] = self.KEtot[i] + self.PEtot[i]
        
    def K(self,i) :
        for j in xrange(self.NP):
            self.KEtot[i] += self.ke(i,j)
        
    def ke(self,i,j):
        return float(0.5 * self.m[j] * ( self.v[i][j][0]*self.v[i][j][0] +  
                                         self.v[i][j][1]*self.v[i][j][1] + 
                                         self.v[i][j][2]*self.v[i][j][2]  ))
    # def P(self,i):
    #     for j in xrange(self.NP):
    #         self.PEtot[i] += (0.5)*self.pe(i,j)
        
    # def pe(self,i,j):
    #     pe_temp = 0.0
    #     self.denom = 0.0
    #     for l in xrange(self.NP):
    #         self.denom = 0.0
    #         if j is not l:
    #             for b in self.xyz:
    #                 self.denom += (self.r[i][l][b] - self.r[i][j][b]) * \
    #                               (self.r[i][l][b] - self.r[i][j][b])
    #             pe_temp += (-1.0)*self.m[j]*self.m[l] / np.sqrt(self.denom)
        
    #     return pe_temp

    def evolve(self):
        for i in xrange(self.nsteps-1):

            # Predict each planets r, v
            for j in xrange(self.NP):
                for k in self.xyz:
                    self.r[i+1][j][k] = self.r[i][j][k] + self.dx * self.fr[i][j][k]
                    self.v[i+1][j][k] = self.v[i][j][k] + self.dx * self.fv[i][j][k]

            # Calculate the force with updated values of r, v
            #for j in xrange(self.NP):
            for k in self.xyz:
                self.F(i+1,k,2)

            # Correct each planet r,v with update force
            for j in xrange(self.NP):
                for k in self.xyz:
                    # self.r[i+1][j][k] = self.r[i][j][k] + self.dx/2.0 * (self.fr[i][j][k]
                    #                                                      + self.fr[i+1][j][k])
                    # self.v[i+1][j][k] = self.v[i][j][k] + self.dx/2.0 * (self.fv[i][j][k]
                    #                                                      + self.fv[i+1][j][k])
                    self.r[i+1][j][k] = self.r[i][j][k] + self.dx * (self.fr[i+1][j][k])
                                                                     
                    self.v[i+1][j][k] = self.v[i][j][k] + self.dx * (self.fv[i+1][j][k])
                                                                         
            # Update the force one more time for the next iteration
            # for j in xrange(self.NP):
            for k in self.xyz:
                self.F(i+1,k,1)
                    
            # Calculate total KE, PE, E
            self.K(i+1)
            self.E(i+1)

    def F(self,i,k,p):
        for j in xrange(self.NP): # planet number
            self.fr[i][j][k] = self.v[i][j][k] 
            self.fv[i][j][k] = self.force(i,j,k,p) #calculate force on j'th planet along k direction
            
    def force(self,i,j,k,p):        # k direction
        self.ff = 0.0             # force on planet j
        
        for l in xrange(self.NP): # planet number l
            self.denom = 0.0
            if j is not l:
                for b in self.xyz:
                    self.denom += (self.r[i][l][b] - self.r[i][j][b]) * \
                                  (self.r[i][l][b] - self.r[i][j][b])

                if k is 0 and p is 1: 
                    self.PEtot[i] += (-0.5)*self.m[j]*self.m[l] / np.sqrt(self.denom)
                    
                self.ff += (self.m[l] * (self.r[i][j][k] - self.r[i][l][k]) / np.power(self.denom,1.5))

        return -1.0*self.ff
