/*
 * Organism.cpp
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#include "../pch.h"
#include "Organism.h"
#include "interface/Genome.h"
#include "../fitness/ScalarFitness.h"

namespace ea {

/**
 * @class Organism
 * Wrapper of Genome with its corresponding Fitness.
 * Organism class represents a Genome has been evaluated. It is the data structure mainly
 * used after evaluation phase and stored after each generation. It also provides Serialize()
 * and Deserialize() functions to enable back-up feature (see BackupHook).
 *
 * @see Population::Evolve()
 */

/**
 * Create an Organism wraps the given Genome with its corresponding Fitness.
 * Default value of this constructor will only be used when reconstructing this object from a BinarySerializer (see Storable).
 * Normal uses should provide both arguments.
 * @param pGenome The Genome to be wrapped.
 * @param pFitness The corresponding Fitness value, usually generated from Evaluator::EvaluateFitness().
 */
Organism::Organism(const GenomePtr& pGenome, const FitnessPtr& pFitness) :
		mGenome(pGenome), mFitness(pFitness) {
}

Organism::~Organism() {
}

/**
 * Get the Fitness object of this Organism.
 * @return The Fitness object.
 */
FitnessPtr Organism::GetFitness() const {
	return mFitness;
}

/**
 * Get the Fitness value of this Organism.
 * The Fitness object stored in this Organism must be a ScalarFitness. Otherwise, exception will be thrown.
 * @return The fitness value.
 */
double Organism::GetFitnessValue() const {
	ScalarFitnessPtr scalar = dynamic_pointer_cast<ScalarFitness>(mFitness);
	if (!scalar)
		throw EA_EXCEPTION(EAException, FITNESS_BAD_CAST,
				"FitnessValue() is not available for non-ScalarFitness object.");
	return scalar->GetValue();
}

/**
 * Get the Genome wrapped in this Organism.
 * @return The wrapped Genome.
 */
const GenomePtr& Organism::GetGenome() const {
	return mGenome;
}

/**
 * Whether this Organism is worse than the given one.
 * This function calls Fitness::operator<() internally.
 * @param pOther The other Organism to be compared.
 * @return **true** if this is worse than pOther.
 */
bool Organism::operator <(const Organism& pOther) {
	return this->mFitness < pOther.mFitness;
}
/**
 * Whether this Organism is better than the given one.
 * This function calls Fitness::operator>() internally.
 * @param pOther The other Organism to be compared.
 * @return **true** if this is better than pOther.
 */
bool Organism::operator >(const Organism& pOther) {
	return this->mFitness > pOther.mFitness;
}

void Organism::DoSerialize(ostream& pStream) const {
	Write(pStream, mGenome);
	Write(pStream, mFitness);
}

void Organism::DoDeserialize(istream& pStream) {
	mGenome = Read<GenomePtr>(pStream);
	mFitness = Read<FitnessPtr>(pStream);
}

} /* namespace ea */
