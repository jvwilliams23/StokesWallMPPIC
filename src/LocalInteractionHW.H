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
    Foam::LocalInteractionHW

Description
    Patch interaction specified on a patch-by-patch basis

\*---------------------------------------------------------------------------*/

#ifndef LocalInteractionHW_H
#define LocalInteractionHW_H

#include "PatchInteractionModel.H"
#include "patchInteractionDataListHW.H"
#include "Switch.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
/*---------------------------------------------------------------------------*\
                      Class LocalInteractionHW Declaration
\*---------------------------------------------------------------------------*/

template<class CloudType>
class LocalInteractionHW
:
    public PatchInteractionModel<CloudType>
{
    // Private data

        //- List of participating patches
        const patchInteractionDataListHW patchData_;


        // Counters for particle fates

            //- Number of parcels escaped
            List<label> nEscape_;

            //- Mass of parcels escaped
            List<scalar> massEscape_;

            //- Number of parcels stuck to patches
            List<label> nStick_;

            //- Mass of parcels stuck to patches
            List<scalar> massStick_;


        //- Flag to output data as fields
        Switch writeFields_;

        //- Mass escape field
        autoPtr<volScalarField> massEscapePtr_;

        //- Mass stick field
        autoPtr<volScalarField> massStickPtr_;


public:

    //- Runtime type information
    TypeName("localInteractionHW");


    // Constructors

        //- Construct from dictionary
        LocalInteractionHW(const dictionary& dict, CloudType& owner);

        //- Construct copy from owner cloud and patch interaction model
        LocalInteractionHW(const LocalInteractionHW<CloudType>& pim);

        //- Construct and return a clone using supplied owner cloud
        virtual autoPtr<PatchInteractionModel<CloudType>> clone() const
        {
            return autoPtr<PatchInteractionModel<CloudType>>
            (
                new LocalInteractionHW<CloudType>(*this)
            );
        }


    //- Destructor
    virtual ~LocalInteractionHW();


    // Member Functions

        //- Return access to the massEscape field
        volScalarField& massEscape();

        //- Return access to the massStick field
        volScalarField& massStick();

        //- Apply velocity correction
        //  Returns true if particle remains in same cell
        virtual bool correct
        (
            typename CloudType::parcelType& p,
            const polyPatch& pp,
            bool& keepParticle
        );


        // I-O

            //- Write patch interaction info to stream
            virtual void info(Ostream& os);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "LocalInteractionHW.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
