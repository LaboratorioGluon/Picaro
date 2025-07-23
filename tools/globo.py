import math

# Constants
R = 0.0821 # [L.atm/mok.K] 

class Atmos:
    Ra = 287.058 #[J/(Kg*K)]
    
    def __init__(self):
        self.layers = [
            # Alt[m], C/km, T0[C], p0[Pa]    , rho0[kg/m3]
            [     0 , -6.5,    15, 101325.000 ,     1.225  ], # Troposfera
            [ 11000 ,  0.0, -56.5,  22635.000 ,     0.3639 ], # Tropopausa
            [ 20000 ,  1.0, -56.5,   5474.900 ,     0.0880 ], # Stratosfera
            [ 32000 ,  2.8, -44.5,    868.020 ,     0.0132 ], # Stratosfera
            [ 47000 ,  0.0,  -2.5,    110.910 ,     0.0014 ], # Stratopause
            [ 51000 , -2.8,  -2.5,     66.939 ,     0.0009 ], # Mesosfera
            [ 71000 , -2.0, -58.5,     3.9564 ,     0.0001 ], # Mesosfera
            [ 84852 ,  0.0, -86.2,          0 ,          0 ]
        
        ]

    def h(self, h):
        g = -9.81
        
        layer = 0
        while h >= self.layers[layer][0]:
            layer = layer+1
        layer = layer-1
        baseAlt, cRate, t0, p0, rho0 = self.layers[layer]
        cRate = cRate/1000 #[C/m]
        t0 = t0 + 273.15 #[K]
        diffAlt = (h - baseAlt)
        T = t0 + cRate*diffAlt
        if cRate != 0:
            p = p0*(T/t0)**(g/(self.Ra*cRate))
        else:
            p = p0 * math.exp(g*diffAlt/(self.Ra*T))
        rho = p/(self.Ra*T)
        #print(f"Layer {layer}")
        print(f"Temp: {T} K -> {T - 273.15} C")
        print(f"Pression: {p} Pa")
        print(f"Densidad: {rho} kg/m3")

        return (T, p, rho)

    # Returns at which height the pressure is the provided
    def p(self, p):
        g = -9.81
        layer = 0
        while p <= self.layers[layer][3]:
            layer += 1
        layer -= 1
        baseAlt, cRate, t0, p0, rho0 = self.layers[layer]
        t0 = t0 + 273.15
        cRate = cRate/1000
        if cRate == 0:
            diffAlt = math.log(p/p0)*t0*self.Ra/g
        else:
            T = t0*((p/p0)**(cRate*self.Ra/g))
            print(T)
            diffAlt = (T - t0)/cRate
        print(f"Required pressure at {layer}")
        print(f"Alt: {baseAlt + diffAlt}")
        return baseAlt + diffAlt

    # Returns at which height the rho is the provided
    def rho(self, rho):
        g = -9.81
        layer = 0
        while rho <= self.layers[layer][4]:
            layer += 1
        layer -= 1
        baseAlt, cRate, t0, p0, rho0 = self.layers[layer]
        t0 = t0 + 273.15
        cRate = cRate/1000
        if cRate == 0:
            diffAlt = (self.Ra * t0/g)*math.log(rho/rho0)
        else:
            N = (g/(cRate*self.Ra))-1
            diffAlt = 1/cRate * (t0 * ((rho/rho0)**(1/N) - 1))
        print(f"Required rho at {layer}")
        print(f"Alt: {baseAlt + diffAlt}")
        return baseAlt + diffAlt

class Balloon:
    
    # maxV [m3], weight [Kg]
    def __init__(self, maxV, weight):
        self.maxV = maxV
        self.emptyWeight = weight

    def setFill(self, gas, volumeAt0):
        # [Kg/m3] | [g/L]
        densities = {
            "H" : 0.08988,
            "He": 0.166
        }
        
        self.gasMass = densities[gas]*volumeAt0
        self.gasVolume0 = volumeAt0

    def getFloatRho(self):
        return (self.gasMass + self.emptyWeight + self.payload)/self.maxV

    def setPayload(self, payload):
        self.payload = payload

    def simulate(self, T, P, airRho):
        g = 9.81
        F = airRho * self.gasVolume0 * g
        R_he = 2077.0 
        R_air = 287.058

        he_rho = P/(R_he*T)
        weight = self.weight + self.gasMass



bal = Balloon(0.23, 50e-3)

atm = Atmos()
atm.h(30000)

atm.p(1171.2)
atm.rho(0.018)

bal.setFill("H", 0.06)
bal.setPayload(0.015)
print(bal.getFloatRho())
