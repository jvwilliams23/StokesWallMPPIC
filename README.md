# StokesWallMPPIC
OpenFOAM-6 code to model particle-wall rebound damping due to lubrication forces. 

The coefficient of restitution (velocity after collision/velocity before collision), varies with a sigmoid as the Stokes number increases, as given by Legendre et al. (2005).


<img src="StokesNumVsCoeffRest.png" width="800" />

In our paper (Williams et al., 2022), we used this to mimic aerosol deposition in a patient respiratory tract, as we treat the lubrication film as acting in a similar way as the mucus layer in a human airway. Therefore, the material properties used in our example below are representative of this case.

## Usage
The library can be linked to the MPPIC solver at run-time by adding the following to the controlDict
```bash
libs 
( 
  "libLocalInteractionModelHW.so" 
);
```
The Stokes wall condition can then be applied using the patchInteractionModel subclass:
```
    patchInteractionModel localInteractionHW;

    localInteractionHWCoeffs
    {
        patches
        (
            walls
            {
                type rebound;
                e    0.92; /* Coefficient of restitution for particle-dry wall collision */
                mu   0.35; /* Particle sliding friction coefficient */
                rhoF	1000.; /* Density of lubrication film */
                muP 	0.056; /* Viscosity of lubrication film */
                coeffStc 35.; /* Critical Stokes number from Legendre et al. */
                eCutoffLog 	-3.6; /* Cutoff Stokes number for 'sticking' condition to be applied (corresponds to St = 10) */
            }
	          "inlet|outlet"
            {
                type escape;
            }
        );
    }
```
## Citation
If you use this, please cite our implementation as well as the studies by Legendre et al. as follows
```
@article{williams2022effect,
  title={Effect of patient inhalation profile and airway structure on drug deposition in image-based models with particle-particle interactions},
  author={Williams, Josh and Kolehmainen, Jari and Cunningham, Steve and Ozel, Ali and Wolfram, Uwe},
  journal={International Journal of Pharmaceutics},
  volume={612},
  pages={121321},
  year={2022},
  publisher={Elsevier}
}

@article{legendre2005experimental,
  title={Experimental study of a drop bouncing on a wall in a liquid},
  author={Legendre, Dominique and Daniel, Claude and Guiraud, Pascal},
  journal={Physics of Fluids},
  volume={17},
  number={9},
  pages={097105},
  year={2005},
  publisher={American Institute of Physics}
}

@article{legendre2006note,
  title={A note on the modelling of the bouncing of spherical drops or solid spheres on a wall in viscous fluid},
  author={Legendre, Dominique and Zenit, Roberto and Daniel, Claude and Guiraud, Pascal},
  journal={Chemical engineering science},
  volume={61},
  number={11},
  pages={3543--3549},
  year={2006},
  publisher={Elsevier}
}

```

## References
Legendre, D., Daniel, C. and Guiraud, P., 2005. Experimental study of a drop bouncing on a wall in a liquid. Physics of Fluids, 17(9), p.097105.

Legendre, D., Zenit, R., Daniel, C. and Guiraud, P., 2006. A note on the modelling of the bouncing of spherical drops or solid spheres on a wall in viscous fluid. Chemical engineering science, 61(11), pp.3543-3549.

Williams, J., Kolehmainen, J., Cunningham, S., Ozel, A. and Wolfram, U., 2022. Effect of patient inhalation profile and airway structure on drug deposition in image-based models with particle-particle interactions. International Journal of Pharmaceutics, 612, p.121321.
