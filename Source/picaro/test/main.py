import numpy as np
import matplotlib.pyplot as plt
import matplotlib as mpl

mpl.rcParams['figure.dpi'] = 300

# --- Define Unit Conversion Functions (to base units: Seconds, Amperes) ---
# Time conversions to seconds
SEC_TO_S = lambda x: x         # seconds to seconds (identity)
MINS_TO_S = lambda x: x * 60   # minutes to seconds

# Current conversions to Amperes
MA_TO_A = lambda x: x / 1000       # milliAmperes to Amperes
UA_TO_A = lambda x: x / 1_000_000  # microAmperes to Amperes

# --- Simulation Parameters ---
SIM_TIME = 3600*24 
dt = 0.1      

# --- Define Power Times Table ---
# Each row: [Duration in seconds, Current in Amperes]
POWER_TIMES = np.array([
    [ SEC_TO_S(1),   MA_TO_A(100) ],
    [ MINS_TO_S(5),  UA_TO_A(10)  ]   
])

# --- Calculate Cumulative Times for one cycle ---
POWER_TIMES_ACUM = np.cumsum(POWER_TIMES[:, 0]) 
TOTAL_CYCLE_DURATION = POWER_TIMES_ACUM[-1] 

print(f"Power Cycle Durations (cumulative, seconds): {POWER_TIMES_ACUM}")
print(f"Total Cycle Duration (seconds): {TOTAL_CYCLE_DURATION}")

# --- Capacitor Class ---
class CAP:
    def __init__(self, C_farads, initial_voltage=0.0):
        self.C = C_farads  # Capacitance in Farads
        self.Q = self.C * initial_voltage # Initial charge in Coulombs
        self.R = 0.8
    
    def addVoltage(self, voltage_V, maxCurrent, dt_s):
        # current_A is in Amperes, dt_s is in seconds
        # Q will be in Coulombs
        current_A = (voltage_V - self.V())/self.R*dt_s
        if current_A > maxCurrent:
            current_A = maxCurrent
        self.Q += current_A * dt_s

        return current_A
    
    def extractCurrent(self, current_A, dt_s):
        # current_A is in Amperes, dt_s is in seconds
        # Q will be in Coulombs
        self.Q -= current_A * dt_s
        
    def V(self):
        # Returns voltage in Volts
        return self.Q / self.C

# --- Simulation Setup ---
tlist = np.arange(0, SIM_TIME, dt) # Time points for simulation
CList = np.empty_like(tlist)       # Array to store capacitor voltage at each time step
CIList = np.empty_like(tlist)       # Array to store capacitor voltage at each time step
CVList = np.empty_like(tlist)
systemCurrent = np.empty_like(tlist)

superCap = CAP(10, initial_voltage=0.0) 

solarPanelVoltage = 1.5
solarPanelCurrent = 0.3

# --- Simulation Loop ---
for i, t in enumerate(tlist):
    inPeriod = np.remainder(t, TOTAL_CYCLE_DURATION) # Current position within the cycle
    
    zone_idx = np.argmax(POWER_TIMES_ACUM > inPeriod) 

    current_to_apply = POWER_TIMES[zone_idx][1]
    systemCurrent[i] = current_to_apply


    maxCcurrent = solarPanelCurrent - current_to_apply
    CIList[i] = superCap.addVoltage(1.5, maxCcurrent, dt) 
    CVList[i] = superCap.V()


# --- Plotting Results ---
plt.figure(figsize=(10, 6))
plt.plot(tlist, systemCurrent)
plt.plot(tlist, CIList)
plt.plot(tlist, CVList)
plt.title('Capacitor Voltage Over Time with Variable Current Load')
plt.xlabel('Time (seconds)')
plt.ylabel('Capacitor Voltage (Volts)')
plt.grid(True)
plt.show()
