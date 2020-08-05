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

\*---------------------------------------------------------------------------*/

#include "patchInteractionDataHW.H"
#include "dictionaryEntry.H"
#include "PatchInteractionModel.H"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

Foam::patchInteractionDataHW::patchInteractionDataHW()
:
    interactionTypeName_("unknownInteractionTypeName"),
    patchName_("unknownPatch"),
    e_(0.0),
    mu_(0.0),
    rhoF_(0.0),
    eCutoffLog_(0.0),
    muP_(0.0),
    coeffStc_(0.0)
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

const Foam::word& Foam::patchInteractionDataHW::interactionTypeName() const
{
    return interactionTypeName_;
}


const Foam::word& Foam::patchInteractionDataHW::patchName() const
{
    return patchName_;
}


Foam::scalar Foam::patchInteractionDataHW::e() const
{
    return e_;
}


Foam::scalar Foam::patchInteractionDataHW::mu() const
{
    return mu_;
}


Foam::scalar Foam::patchInteractionDataHW::rhoF() const
{
    return rhoF_;
}


Foam::scalar Foam::patchInteractionDataHW::eCutoffLog() const
{
    return eCutoffLog_;
}

Foam::scalar Foam::patchInteractionDataHW::muP() const
{
    return muP_;
}


Foam::scalar Foam::patchInteractionDataHW::coeffStc() const
{
    return coeffStc_;
}


// * * * * * * * * * * * * * * * IOstream Operators  * * * * * * * * * * * * //

Foam::Istream& Foam::operator>>
(
    Istream& is,
    patchInteractionDataHW& pid
)
{
    is.check("Istream& operator>>(Istream&, patchInteractionDataHW&)");

    const dictionaryEntry entry(dictionary::null, is);

    pid.patchName_ = entry.keyword();
    entry.lookup("type") >> pid.interactionTypeName_;
    pid.e_ = entry.lookupOrDefault<scalar>("e", 1.0);
    pid.mu_ = entry.lookupOrDefault<scalar>("mu", 0.0);
    pid.rhoF_ = entry.lookupOrDefault<scalar>("rhoF", 0.0);
    pid.eCutoffLog_ = entry.lookupOrDefault<scalar>("eCutoffLog", 0.0);
    pid.muP_ = entry.lookupOrDefault<scalar>("muP", 0.0);
    pid.coeffStc_ = entry.lookupOrDefault<scalar>("coeffStc", 0.0);

    return is;
}


// ************************************************************************* //
