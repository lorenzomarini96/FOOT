# Setup

## ∆E-TOF

**Sorce**: *Charge identification of nuclear fragments with the FOOT Time-Of-Flight system (A.C. Kraan, R. Zarrella, A. Alexandrov et al./ Nuclear Inst. and Methods in Physics Research, A 1001 (2021) 165206)*

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

### Start Counter

- The SC is used to provide the **rate** of the primary particle beam and
their start time.
- It is the first detector encountered by the beam
and is located before the target.
-  It utilizes a thin (250 μm) **foil of EJ-228 plastic scintillator** with an active area of 5 × 5 cm^2.
- The light produced in the SC is collected by 8 groups of six 3 × 3 mm^2 ASD- NUV3S-P SiPMs with 25 μm microcell pitch.
- Each group of SiPMs provides a summed **analogical signal**.

### TOF-Wall 

- The TW, located behind the target, is used to measure the energy loss 𝛥𝐸 of the passing particles and to provide their arrival time.
- It is made of **40 bars of EJ-200 plastic scintillator** arranged in two orthogonal layers of 20 each (for each layer, we had 20 × 20 = 400 hit positions).
- Each bar has an active area of 44 × 2 cm^2 and is 3 mm **thick**.
- The light produced in the bars is collected at each end by **four 3 × 3 mm2 MPPC SiPMs** with 25 μm microcell pitch .
- Each group of four SiPMs provides a **single summed analogical signal** for further processing.
- The signals collected by the SiPMs of both detectors are sampled through **WaveDREAM digitizer boards (WDB)**, integrated in the **WaveDAQ system**.

- The WDBs can sample up to **16 channels** and also provide the supply voltage to the SiPMs of the detectors.

- The readout of the 𝛥𝐸-TOF employs:
    - 8 independent analogue channels (connected to 1 WDB) for the SC;
    - 2 channels for each bar of the TW (80 channels and 7 WDBs in total).

## 𝛥E calibration

- In order to extract the energy loss of the fragments passing through the 𝛥𝐸-TOF system, the raw values for the energy loss were extracted directly from the TW signals.
- The two channels of each bar involved in the event (indicated with the subscripts *A/B*) were processed separately and the charge collected in each of them *Q_{A/B}* was evaluated as the area of the signals.

- Supposing a simple **exponential attenuation along the bar**, the total charge collected was defined as *Q = √(Q_A)x(Q_B)*

- *Q* represents the raw energy loss of particles travelling through the bar. Ideally, it should be independent of the hit position along the bar.

- However, the mean value of the detected charge was asymmetric for the two sides of the bar. They verified that other bars and data sets showed a similar trend. This may be due to differences in total internal reflection efficiency, light absorption along the bar, optical coupling and SiPM gain between the two sides.

- The calibrated **energy loss of a particle** in a given position 𝑖, 𝛥𝐸𝑖, was then defined as the sum of the energy loss in the two layers of the TW:

𝛥𝐸𝑖 = 𝛥𝐸𝑖_𝐹 + 𝛥𝐸𝑖_𝑅

(front layer (𝑙 = 𝐹), )


## Energy resolution

The overall energy resolution of the system can be extracted from the calibrated energy loss values as follows:

- The 𝛥𝐸𝑖 values obtained for each event in all well-calibrated positions of a calibration sample were merged into one ‘global’ distribution to obtain the **energy spectrum of each data set**. 
- Then, we performed a **Gaussian fit** on the resulting distribution to extract the **mean value** µ(𝛥𝐸) and **standard deviation** 𝜎(𝛥𝐸) of each sample.
- The **energy resolution** of the system was defined as the obtained 𝜎(𝛥𝐸) value.

- It is needed to parameterize the contribution to the energy resolution of all the effects, such as photon production, optical transport, electronics, etc.
