/*
 * ArrayGenome.h
 *
 *  Created on: Mar 17, 2017
 *      Author: Bui Quang Minh
 *       Email: minhcly95@outlook.com
 */

#pragma once

#include "../EA/Type/Core.h"
#include "../EA/Type/Array.h"
#include "../core/interface/Genome.h"
#include "../rtoc/Storable.h"
#include "../rtoc/Constructible.h"

namespace ea {

using namespace std;

/**
 * Functionality class for ArrayGenome.
 * Because ArrayGenome is usually being specialized, this class provides basic functionality
 * of an ArrayGenome such as GetGenes() and GetSize().
 * This class also provides a (de)serialization method for ArrayGenome.
 *
 * PermutationGenome also re-uses the functionality of ArrayGenome by inheriting this class.
 *
 * @tparam T Type of genes.
 * @see ArrayGenome
 * @see PermutationGenome
 */
template<class T>
class ArrayGenomeBase: public virtual Storable {
public:
	using GenomeType = vector<T>;

protected:
	GenomeType mGenes;

public:
	/**
	 * Create an ArrayGenomeBase with content copied from the given vector of genes.
	 * @param pGenes A vector of genes of type T.
	 */
	inline ArrayGenomeBase(GenomeType& pGenes) :
			mGenes(pGenes) {
	}
	/**
	 * Create an ArrayGenomeBase with empty vector of genes.
	 */
	inline ArrayGenomeBase() :
			mGenes() {
	}
	inline virtual ~ArrayGenomeBase() {
	}

	/**
	 * Get the size of the array.
	 * @return The size of the array (the number of genes in this ArrayGenome).
	 */
	inline uint GetSize() const {
		return mGenes.size();
	}
	/**
	 * Get the array of genes as reference.
	 * @return The reference of contained array of genes.
	 */
	inline GenomeType& GetGenes() {
		return mGenes;
	}

protected:
	inline virtual void DoSerialize(ostream& pStream) const override {
		Write<vector<T>>(pStream, mGenes);
	}
	inline virtual void DoDeserialize(istream& pStream) override {
		mGenes = Read<vector<T>>(pStream);
	}

};

/**
 * Genome which stores an array of genes of type T.
 *
 * This genome type uses a vector of type T as the representation. Users can define custom type T for gene type,
 * but it should be supported by BinarySerializer (should be primitive type, simple struct, string or Storable).
 *
 * Functionality of this class is provided by ArrayGenomeBase class. Please check GetSize() and GetGenes().
 *
 * It is not recommended that users build a vector outside than input it to the ArrayGenome by using the constructor,
 * because the vector will be copied and it will be costly if the array size is huge.
 * Users should create an empty ArrayGenome using the default constructor instead, then access the vector by calling GetGenes()
 * to get the reference of the contained vector. By this method, the vector will be built in-place.
 *
 * The NameService supports three ArrayGenome types by default via alias names:
 * \ref BoolArrayGenome, \ref IntArrayGenome and \ref DoubleArrayGenome.
 * However, the NameService won't understand the real name "ArrayGenome<int>", therefore please use the above alias names.
 *
 * To enable the NameService to add an ArrayGenome of user type, please define a TypeInfo and add it into the NameService as follow:
 *
 * @code
// Put in namespace level
using UserArrayGenome = ArrayGenome<UserType>;
EA_TYPEINFO_SPECIALIZED_DEFAULT(UserArrayGenome)

// Put in a function
NameService::Add<UserArrayGenome>();
 * @endcode
 *
 * @name{BoolArrayGenome, IntArrayGenome, DoubleArrayGenome}
 *
 * @tparam T Type of genes.
 */
template<class T>
class ArrayGenome: public ArrayGenomeBase<T>, public GenomeClonable<
		ArrayGenome<T>> {
public:
	EA_TYPEINFO_CUSTOM_DECL

	/**
	 * Create an ArrayGenome with content copied from the given vector of genes.
	 * @param pGenes A vector of genes of type T.
	 */
	inline ArrayGenome(typename ArrayGenomeBase<T>::GenomeType& pGenes) :
			ArrayGenomeBase<T>(pGenes) {
	}
	/**
	 * Create an ArrayGenome with empty vector of genes.
	 */
	inline ArrayGenome() :
			ArrayGenomeBase<T>() {
	}
	inline virtual ~ArrayGenome() {
	}

	inline virtual ostream& Print(ostream& os) const override {
		os << "<ArrayGenome>";
		return os;
	}
};

#ifndef DOXYGEN_IGNORE
template<>
ostream& ArrayGenome<bool>::Print(ostream& os) const;
EA_TYPEINFO_SPECIALIZED_DEFAULT(BoolArrayGenome)

template<>
ostream& ArrayGenome<int>::Print(ostream& os) const;
EA_TYPEINFO_SPECIALIZED_DEFAULT(IntArrayGenome)

template<>
ostream& ArrayGenome<double>::Print(ostream& os) const;
EA_TYPEINFO_SPECIALIZED_DEFAULT(DoubleArrayGenome)
#endif

} /* namespace ea */
