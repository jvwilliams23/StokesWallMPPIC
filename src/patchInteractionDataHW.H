/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     | Website:  https://openfoam.org
    \\  /    A nd           | Copyright (C) 2011-2018 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::patchInteractionDataHW

Description
    Helper class for the LocalInteractionHW patch interaction model

\*---------------------------------------------------------------------------*/

#ifndef patchInteractionDataHW_H
#define patchInteractionDataHW_H

#include "Istream.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                    Class patchInteractionDataHW Declaration
\*---------------------------------------------------------------------------*/

// Forward declaration of classes
class patchInteractionDataHW;

// Forward declaration of friend functions
Istream& operator>>
(
    Istream& is,
    patchInteractionDataHW& pid
);


class patchInteractionDataHW
{
    // Private data

        //- Interaction type name
        word interactionTypeName_;

        //- Patch name
        word patchName_;

        //- Elasticity coefficient
        scalar e_;

        //- Restitution coefficient
        scalar mu_;
	
	
	//-Stokes BC params from Legendre 2005 Phys of fluids, 17
	//- Fluid density on patch
	scalar rhoF_;
	
	//- Cutoff restitution coefficient (as a natural log)
	scalar eCutoffLog_;
	
	//- Lubrication fluid viscosity on patch
	scalar muP_;
	
	//- Critical stokes number coefficient
	scalar coeffStc_;
	

public:

    // Constructor

        //- Construct null
        patchInteractionDataHW();


    // Member functions

        // Access

            //- Return const access to the interaction type name
            const word& interactionTypeName() const;

            //- Return const access to the patch name
            const word& patchName() const;

            //- Return const access to the elasticity coefficient
            scalar e() const;

            //- Return const access to the restitution coefficient
            scalar mu() const;
	    
	    //-Return const access to lubrication fluid density
	    scalar rhoF() const;
	    
	    //-Return const access to min restitution coeff cutoff
	    scalar eCutoffLog() const;
	    
	    //-Return const access to lubrication fluid viscosity
	    scalar muP() const;
	    
	    //-Return const access to critical stokes number
	    scalar coeffStc() const;


        // I-O

            //- Istream operator
            friend Istream& operator>>
            (
                Istream& is,
                patchInteractionDataHW& pid
            );
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
