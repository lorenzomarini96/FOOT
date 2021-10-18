# Nuclear Fragmentation measurement for Particle Therapy with FOOT experiment

**Optimization of the trigger system and data acquisition of the FOOT experiment at the CNAO**

<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/foot_logo.jpeg" width="80">

[FOOT (FragmentatiOn Of Target)](https://web.infn.it/foot/) is an applied nuclear physics experiment conceived to conduct high-precision cross section measurements of nuclear fragmentation processes relevant for [particle therapy](https://en.wikipedia.org/wiki/Particle_therapy) and [radiation protection in space](https://www.nasa.gov/sites/default/files/atoms/files/space_radiation_ebook.pdf). These measurements are important to estimate the physical and biological effects of nuclear fragments, which are produced when energetic particle beams penetrate human tissue.

## Goal of FOOT

The goal of the FOOT (FragmentatiOn Of Target) experiment is to measure **double differential cross sections** (in fragment kinetic energy and direction) for nuclear fragmentation processes induced by beams impinging on targets, that are of interest for particle therapy.

## FOOT applications

### Hadrontherapy

<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/med_rad.jpeg" width="160">

Hadrontherapy is a very advanced form of radiotherapy. **Radiotherapy**, alone or associated with surgery and / or chemotherapy, improves local control in various tumor pathologies.

The strength of hadrontherapy resides in the unique physical and radiobiological properties of the **charged** **particles** (hadrons): they can penetrate the tissues with little diffusion and deposit maximum energy just before stopping. This allows the region to be irradiated to be defined very **precisely**. The characteristic peak shape of the energy deposit is called the [Bragg peak](https://en.wikipedia.org/wiki/Bragg_peak) and has become the symbol of hadrontherapy.

### Space Travel 

<img src="https://github.com/lorenzomarini96/FOOT/blob/main/figures/space_rad.jpeg" width="160">

One of the biggest obstacles to **space travel** is the large amount of **radiation** present outside the atmosphere, which would pose a serious risk to both the crew and the electronics. The data provided by FOOT can be used to develop **cosmic radiation protection systems** aboard space shuttles.

## The repository

This repository contains some material on which I have been working in these months together with my thesis supervisor (Dr. Luca Galli). As the trigger manager, he is teaching me many interesting aspects regarding the acquisition trigger of the **FOOT experiment**. 

### Topics

The topics range from **hadrontherapy** to the revelation of charged ions; from [FPGAs](https://en.wikipedia.org/wiki/Field-programmable_gate_array), [Wavedream](https://www.psi.ch/sites/default/files/import/drs/DocumentationEN/elba15.pdf), and acquisition systems to board firmware and [Verilog](https://en.wikipedia.org/wiki/Verilog) language.


Here are some useful links as a reference for basic knowledge and for any further information.

## Keywords
- Nuclear Physics
- Nuclear Fragmentation
- Particle Therapy
- Data acquisition system (DAQ)
- Wavedream board
- Trigger system

## Useful links
- [FOOT](https://web.infn.it/foot/)
- [CNAO](https://fondazionecnao.it)


# Repo Structure
```
FOOT/
├── calibration_w166
│   ├── data
│   ├── figures
│   ├── instrument
│   └── slides
│       ├── figures
│       │   ├── crate_configuration
│       │   └── instruments
│       └── photos
├── clock
└── tofwall

```

# Useful knowledge

## Verilog
- [wkikVerilog](https://it.wikipedia.org/wiki/Verilog)
- [Il linguaggio Verilog](https://www.ge.infn.it/~musico/CourseStuff/VerilogSlides.pdf)
-  [Note sull’utilizzo di Verilog per la prima parte del corso di Architettura degli Elaboratori](http://didawiki.di.unipi.it/lib/exe/fetch.php/informatica/ae/verilog2.pdf)

## Vivado
- [wikiVivado](https://en.wikipedia.org/wiki/Xilinx_Vivado)

## Root

- [root](https://root.cern)
- [tutorial](https://root.cern/doc/master/group__Tutorials.html)
    - [histogram](https://root.cern/doc/master/group__tutorial__hist.html)
    - [tree](https://root.cern/doc/master/group__tutorial__tree.html)
    - [fit](https://root.cern/doc/master/group__tutorial__fit.html)
    - [RooFit](https://root.cern/doc/master/group__tutorial__roofit.html)
    - [graphs](https://root.cern/doc/master/group__tutorial__graphs.html)
    - [Math](https://root.cern/doc/master/group__tutorial__math.html)

# Task Lists
- [x] Neutron Trigger
- [x] Fragmentation Trigger
- [x] Calorimeter Trigger
- [ ] Data acquisition at CNAO
- [ ] Write Thesis 


# Thanks
- Dr. Luca Galli (INFN)
- Prof. Maria Giuseppina Bisogni (INFN)
- Dr. Marco Francesconi (INFN)
- ?
