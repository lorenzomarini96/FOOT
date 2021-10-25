# Setup

**Sorce**: 
1. *Charge identification of nuclear fragments with the FOOT Time-Of-Flight system (A.C. Kraan, R. Zarrella, A. Alexandrov et al./ Nuclear Inst. and Methods in Physics Research, A 1001 (2021) 165206)*
2. *Fragment charge identification technique with a plastic scintillator detector using clinical carbon beams (L. Galli, A.C. Kraan, E. Ciarrocchi et al.)*
3. https://docs.google.com/spreadsheets/d/1hk4Tdgfkv0YjdsjldSWQQZtwZSucif55sulnaugNLGw/edit#gid=334484356

# Full System
<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/full_system.png" width="900">

# ∆E-TOF

- A crucial component of the FOOT apparatus is the 𝛥𝐸-TOF system, designed to identify the **charge** of the fragments using **plastic scintillators** to measure the **energy** deposited and the **time of flight** with respect to a start counter.

- It is designed to measure energy loss and time- of-flight of nuclear fragments produced in particle collisions in thin targets in order to extract their charge and **velocity**.

- The 𝛥𝐸-TOF system is composed of:
    - a **start counter**, providing the start time for the time-of-flight;
    
    -  a 40 × 40 cm^2 wall of thin plastic scintillator bars, providing the arrival time and energy loss of the fragments passing through the detector.
    
- Particle charge discrimination can be achieved by correlating the energy loss in the scintillator bars with the measured time-of-flight.

- To reach this goal, the system should achieve for such nuclei an **accuracy** in energy loss and TOF measurements of at most 5% and 100 ps, respectively.

- The 𝛥𝐸-TOF system consists of two **scintillation detectors**: 
    - the *Start Counter* (SC)
    - the *TOF-Wall* (TW).

<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/DeltaE_TOF.png" width="700">

## Start Counter

- The SC is used to provide the **rate** of the primary particle beam and
their start time.

- It is the first detector encountered by the beam
and is located before the target.

-  It utilizes a thin (250 μm) **foil of EJ-228 plastic scintillator** with an active area of 5 × 5 cm^2.

- The light produced in the SC is collected by 8 groups of six 3 × 3 mm^2 ASD- NUV3S-P SiPMs with 25 μm microcell pitch.

- Each group of SiPMs provides a summed **analogical signal**.

## TOF-Wall 

- The TW, located behind the target, is used to measure the energy loss 𝛥𝐸 of the passing particles and to provide their arrival time.

- It is made of **40 bars of EJ-200 plastic scintillator** arranged in two orthogonal layers of 20 each (for each layer, we had 20 × 20 = 400 hit positions).

- Each bar has an active area of 44 × 2 cm^2 and is 3 mm **thick**.

<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/tofwall.png" width="400">

- At each end of each bar, the two series of two SiPMs were connected in parallel.

- The light produced in the bars is collected at each end by **four 3 × 3 mm2 MPPC SiPMs** with 25 μm microcell pitch .

- Each group of four SiPMs provides a **single summed analogical signal** for further processing.

- The **output signal** of each side of each bar was input to a waveform digitizer board, WaveDREAM, hosted in the WaveDAQ integrated trigger and data acquisition system.

- The signals collected by the SiPMs of both detectors are sampled through **WaveDREAM digitizer boards (WDB)**, integrated in the **WaveDAQ system**.

- Each module end was read out by one channel of the WaveDREAM board, that provided also the **bias voltage** for the SiPMs

- The WDBs can sample up to **16 channels** and also provide the supply voltage to the SiPMs of the detectors.

- The readout of the 𝛥𝐸-TOF employs:
    - 8 independent analogue channels (connected to 1 WDB) for the SC;
    - 2 channels for each bar of the TW (80 channels and 7 WDBs in total).
    
### TOF *x*, TOF *y*    
<p float="left">
<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/tofx.png" width="300">
<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/tofy.png" width="325">
</p>


### Connection Map (*x*-view, *y*-view)
<p float="left">
<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/connection_mapx.png" width="300">
<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/connection_mapy.png" width="313">
</p>



## 𝛥E calibration
- Calibrate the energy response of the detector
- In order to extract the energy loss of the fragments passing through the 𝛥𝐸-TOF system, the raw values for the energy loss were extracted directly from the TW signals.
- When charged particles hit a plastic scintillator, scintillation photons are produced. Due to several effects (attenuation, photons exiting the bars on the sides by refraction, absorption depending on the wrapping material), only a fraction of the scintillation photons reaches the photo-detectors at the ends of the scintillator bar, depending on the interaction position.

- The two channels of each bar involved in the event (indicated with the subscripts *A/B*) were processed separately and the charge collected in each of them *Q_{A/B}* was evaluated as the area of the signals.

- Supposing a simple **exponential attenuation along the bar**, the total charge collected was defined as *Q = √(Q_A)x(Q_B)*

-  For each event, the total charge 𝑄𝑖,𝐷 (where 𝑖 = 1,2 refers to the module) collected by each module is evaluated from the collected charge (integral of the waveform) on each side 𝑄𝑙,𝑖,𝐷 and 𝑄𝑟,𝑖,𝐷, where 𝑙 and 𝑟 are the *left* and *right* side of the bar, respectively, as follows: 𝑄𝑖,𝐷 = √(𝑄𝑙,𝑖,𝐷 ⋅ 𝑄𝑟,𝑖,𝐷)
- 𝑄𝑖,𝐷 represents the raw energy loss of particles travelling through the bar. Ideally, it should be independent of the hit position along the bar.

- However, the mean value of the detected charge was asymmetric for the two sides of the bar. They verified that other bars and data sets showed a similar trend. This may be due to differences in total internal reflection efficiency, light absorption along the bar, optical coupling and SiPM gain between the two sides.

- The **calibration procedure** consists of relating the collected charge 𝑄𝑖,𝐷 with a value of deposited energy 𝛥𝐸𝑖,𝐷.

- The calibrated **energy loss of a particle** in a given position 𝑖, 𝛥𝐸𝑖, was then defined as the sum of the energy loss in the two layers of the TW:

𝛥𝐸𝑖 = 𝛥𝐸𝑖_𝐹 + 𝛥𝐸𝑖_𝑅

(front layer (𝑙 = 𝐹), )


## Energy resolution

The overall energy resolution of the system can be extracted from the calibrated energy loss values as follows:

- The 𝛥𝐸𝑖 values obtained for each event in all well-calibrated positions of a calibration sample were merged into one ‘global’ distribution to obtain the **energy spectrum of each data set**. 
- Then, we performed a **Gaussian fit** on the resulting distribution to extract the **mean value** µ(𝛥𝐸) and **standard deviation** 𝜎(𝛥𝐸) of each sample.
- The **energy resolution** of the system was defined as the obtained 𝜎(𝛥𝐸) value.

- It is needed to parameterize the contribution to the energy resolution of all the effects, such as photon production, optical transport, electronics, etc.
