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

#include "LocalInteractionHW.H"

// * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * * //

template<class CloudType>
Foam::LocalInteractionHW<CloudType>::LocalInteractionHW
(
    const dictionary& dict,
    CloudType& cloud
)
:
    PatchInteractionModel<CloudType>(dict, cloud, typeName),
    patchData_(cloud.mesh(), this->coeffDict()),
    nEscape_(patchData_.size(), 0),
    massEscape_(patchData_.size(), 0.0),
    nStick_(patchData_.size(), 0),
    massStick_(patchData_.size(), 0.0),
    writeFields_(this->coeffDict().lookupOrDefault("writeFields", false)),
    massEscapePtr_(nullptr),
    massStickPtr_(nullptr)
{
    if (writeFields_)
    {
        word massEscapeName(this->owner().name() + ":massEscape");
        word massStickName(this->owner().name() + ":massStick");
        Info<< "    Interaction fields will be written to " << massEscapeName
            << " and " << massStickName << endl;

        (void)massEscape();
        (void)massStick();
    }
    else
    {
        Info<< "    Interaction fields will not be written" << endl;
    }

    // check that interactions are valid/specified
    forAll(patchData_, patchi)
    {
        const word& interactionTypeName =
            patchData_[patchi].interactionTypeName();
        const typename PatchInteractionModel<CloudType>::interactionType& it =
            this->wordToInteractionType(interactionTypeName);

        if (it == PatchInteractionModel<CloudType>::itOther)
        {
            const word& patchName = patchData_[patchi].patchName();
            FatalErrorInFunction
                << "Unknown patch interaction type "
                << interactionTypeName << " for patch " << patchName
                << ". Valid selections are:"
                << this->PatchInteractionModel<CloudType>::interactionTypeNames_
                << nl << exit(FatalError);
        }
    }
}


template<class CloudType>
Foam::LocalInteractionHW<CloudType>::LocalInteractionHW
(
    const LocalInteractionHW<CloudType>& pim
)
:
    PatchInteractionModel<CloudType>(pim),
    patchData_(pim.patchData_),
    nEscape_(pim.nEscape_),
    massEscape_(pim.massEscape_),
    nStick_(pim.nStick_),
    massStick_(pim.massStick_),
    writeFields_(pim.writeFields_),
    massEscapePtr_(nullptr),
    massStickPtr_(nullptr)
{}


// * * * * * * * * * * * * * * * * Destructor  * * * * * * * * * * * * * * * //

template<class CloudType>
Foam::LocalInteractionHW<CloudType>::~LocalInteractionHW()
{}


// * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * * //

template<class CloudType>
Foam::volScalarField& Foam::LocalInteractionHW<CloudType>::massEscape()
{
    if (!massEscapePtr_.valid())
    {
        const fvMesh& mesh = this->owner().mesh();

        massEscapePtr_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    this->owner().name() + ":massEscape",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimMass, 0.0)
            )
        );
    }

    return massEscapePtr_();
}


template<class CloudType>
Foam::volScalarField& Foam::LocalInteractionHW<CloudType>::massStick()
{
    if (!massStickPtr_.valid())
    {
        const fvMesh& mesh = this->owner().mesh();

        massStickPtr_.reset
        (
            new volScalarField
            (
                IOobject
                (
                    this->owner().name() + ":massStick",
                    mesh.time().timeName(),
                    mesh,
                    IOobject::READ_IF_PRESENT,
                    IOobject::AUTO_WRITE
                ),
                mesh,
                dimensionedScalar("zero", dimMass, 0.0)
            )
        );
    }

    return massStickPtr_();
}


template<class CloudType>
bool Foam::LocalInteractionHW<CloudType>::correct
(
    typename CloudType::parcelType& p,
    const polyPatch& pp,
    bool& keepParticle
)
{
    label patchi = patchData_.applyToPatch(pp.index());

    if (patchi >= 0)
    {
        vector& U = p.U();
        bool& active = p.active();

        typename PatchInteractionModel<CloudType>::interactionType it =
            this->wordToInteractionType
            (
                patchData_[patchi].interactionTypeName()
            );

        switch (it)
        {
            case PatchInteractionModel<CloudType>::itNone:
            {
                return false;
            }
            case PatchInteractionModel<CloudType>::itEscape:
            {
                scalar dm = p.mass()*p.nParticle();

                keepParticle = false;
                active = false;
                U = Zero;
                nEscape_[patchi]++;
                massEscape_[patchi] += dm;
                if (writeFields_)
                {
                    label pI = pp.index();
                    label fI = pp.whichFace(p.face());
                    massEscape().boundaryFieldRef()[pI][fI] += dm;
                }
                break;
            }
            case PatchInteractionModel<CloudType>::itStick:
            {
                scalar dm = p.mass()*p.nParticle();

                keepParticle = true;
                active = false;
                U = Zero;
                nStick_[patchi]++;
                massStick_[patchi] += dm;
                if (writeFields_)
                {
                    label pI = pp.index();
                    label fI = pp.whichFace(p.face());
                    massStick().boundaryFieldRef()[pI][fI] += dm;
                }
                break;
            }
            case PatchInteractionModel<CloudType>::itRebound:
            {
                keepParticle = true;
		
		scalar dm = p.mass()*p.nParticle();
		scalar dp = p.d();
		scalar rhoP = p.rho();
		scalar CMinf = 0.5;
		
		// Stokes condition params
		scalar coeffRestLogChosen = patchData_[patchi].eCutoffLog();
		scalar coeffRestMax = patchData_[patchi].e();
		scalar coeffStc = patchData_[patchi].coeffStc(); 
		scalar coeffRhof = patchData_[patchi].rhoF();
		scalar coeffMu = patchData_[patchi].muP();

                vector nw;
                vector Up;		

                this->owner().patchData(p, pp, nw, Up);

                // Calculate motion relative to patch velocity
                U -= Up;

                scalar Un = U & nw;
                vector Ut = U - Un*nw;
		
		if ( coeffStc != 0.0 && active)
		{
		
			scalar Stokes = ((rhoP+CMinf*coeffRhof)*Un*dp)/(9.0*coeffMu);
			coeffRestLogChosen = log(coeffRestMax)-coeffStc/Stokes;
			
			if (coeffRestLogChosen < -5.3)
			{
				active = false;
                		U = Zero;
                		nStick_[patchi]++;
                		massStick_[patchi] += dm;
                		if (writeFields_)
                		{
                    			label pI = pp.index();
                    			label fI = pp.whichFace(p.face());
                    			massStick().boundaryFieldRef()[pI][fI] += dm;
                		}
			}
			else
			{
				if (Un > 0)
                		{
				    //damp normal velocity if coeffRest not small enough to delete
                  	  	    U -= (1.0 + exp(coeffRestLogChosen) )*Un*nw; //JW 20/05/20
               			}

                		U -= patchData_[patchi].mu()*Ut;

                		// Return velocity to global space
				U += Up;
			}
		}
		else
		{

                	if (Un > 0)
                	{
                   		U -= (1.0 + patchData_[patchi].e())*Un*nw;
                	}

                	U -= patchData_[patchi].mu()*Ut;

                	// Return velocity to global space
                	U += Up;
		}

                break;
            }
		
            default:
            {
                FatalErrorInFunction
                    << "Unknown interaction type "
                    << patchData_[patchi].interactionTypeName()
                    << "(" << it << ") for patch "
                    << patchData_[patchi].patchName()
                    << ". Valid selections are:" << this->interactionTypeNames_
                    << endl << abort(FatalError);
            }
        }

        return true;
    }

    return false;
}


template<class CloudType>
void Foam::LocalInteractionHW<CloudType>::info(Ostream& os)
{
    // retrieve any stored data
    labelList npe0(patchData_.size(), 0);
    this->getModelProperty("nEscape", npe0);

    scalarList mpe0(patchData_.size(), 0.0);
    this->getModelProperty("massEscape", mpe0);

    labelList nps0(patchData_.size(), 0);
    this->getModelProperty("nStick", nps0);

    scalarList mps0(patchData_.size(), 0.0);
    this->getModelProperty("massStick", mps0);

    // accumulate current data
    labelList npe(nEscape_);
    Pstream::listCombineGather(npe, plusEqOp<label>());
    npe = npe + npe0;

    scalarList mpe(massEscape_);
    Pstream::listCombineGather(mpe, plusEqOp<scalar>());
    mpe = mpe + mpe0;

    labelList nps(nStick_);
    Pstream::listCombineGather(nps, plusEqOp<label>());
    nps = nps + nps0;

    scalarList mps(massStick_);
    Pstream::listCombineGather(mps, plusEqOp<scalar>());
    mps = mps + mps0;


    forAll(patchData_, i)
    {
        os  << "    Parcel fate (number, mass)      : patch "
            <<  patchData_[i].patchName() << nl
            << "      - escape                      = " << npe[i]
            << ", " << mpe[i] << nl
            << "      - stick                       = " << nps[i]
            << ", " << mps[i] << nl;
    }

    if (this->writeTime())
    {
        this->setModelProperty("nEscape", npe);
        nEscape_ = 0;

        this->setModelProperty("massEscape", mpe);
        massEscape_ = 0.0;

        this->setModelProperty("nStick", nps);
        nStick_ = 0;

        this->setModelProperty("massStick", mps);
        massStick_ = 0.0;
    }
}


// ************************************************************************* //
