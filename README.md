# Interface-circuit-for-Inductive-Power-Transmission-

A 1 MHz, 1 V p-p sine wave is to be generated using Direct Digital Synthesis to drive wireless charging using Inductive Wireless Power 
Transmission. To overcome the limitations of, the DDS signal, the signal is to be processed before it can be used to drive the coil.
A bandpass filter followed by a voltage follower is a possible solution. In today’s world, wireless power charging is complementing or 
slowly replacing charging wires. The power transmission could be wired or wireless. Wireless Power Transfer (WPT) has a wide range of 
applications, like electronic devices, biomedical implants, charging of electric buses/trains, electronic toothbrushes etc. 
One of the methods used for WPT is achieved by inductive power transmission through electromagnetic induction. Here, the primary coil 
(transmitter) generates a magnetic field in the secondary coil (receiver), when there is a change in the current passing through the 
primary coil. The induced voltage can be used to charge the battery or other circuits. Using primary and secondary coupled circuits can
increase the induced magnetic field. 

In this project, Direct Digital Synthesis method is used to generate an analog signal (sine wave). This generates a time-varying signal 
in digital form then is converted by an internal DAC. The generated sinusoidal signal has an offset and noise which needs to be removed,
so that it can be used for WPT. Arduino platform, is used to write the code for generation of sine wave. Arduino consists of a programmable 
circuit board and software or IDE (Integrated Development Environment) 

Band pass filter (BPF) is designed to allow 1 MHz frequency to pass through. 3 different designs were simulated to implement a band pass 
filter using a combination of LPF and HPF. 
1) Pure Resistor- Capacitor network 

2) BPF using op-amp LT1007 
An operational amplifier was selected to implement LPF and HPF to form the BPF. The following parameters were considered for the choice 
of op-amp: 
 High Slew Rate  High Bandwidth  Low Bias Current  Low Offset Voltage  Low Noise

3) BPF using op-amp LT1007, Butterworth filter 
In order to get a narrower band for the BPF, a LPF-HPF combination was realized in Butterworth filter configuration. 
