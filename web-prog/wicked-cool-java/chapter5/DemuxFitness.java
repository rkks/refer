package com.wickedcooljava.sci;

import org.jgap.Chromosome;
import org.jgap.Configuration;
import org.jgap.Gene;
import org.jgap.Genotype;
import org.jgap.InvalidConfigurationException;
import org.jgap.impl.DefaultConfiguration;
import org.jgap.impl.IntegerGene;

import cern.colt.bitvector.BitVector;

/*
 Example code from Wicked Cool Java (No Starch Press)
 Copyright (C) 2005 Brian D. Eubanks

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

 Note: The LGPL licence can be found online at http://www.gnu.org

 */
/**
 * Demonstration using the JGAP genetic algorithms API to evolve a truth table.
 */
public class DemuxFitness extends org.jgap.FitnessFunction {
	private SimpleTruthTable tt;

	public DemuxFitness() {
		// this is our target, what we want to evolve to
		tt = new SimpleTruthTable(3, 4);
		tt.store(4, 1); // 100->0001
		tt.store(5, 2); // 101->0010
		tt.store(6, 4); // 110->0100
		tt.store(7, 8); // 111->1000
	}

	public int correctBits(int data) {
		BitVector vecValue = new BitVector(new long[] { data }, 32);
		BitVector target = tt.getTruthMatrix().toBitVector();
		// we can find the number of correct bits using:
		// count(not(xor(target,vecValue)))
		vecValue.xor(target);
		vecValue.not();
		return vecValue.cardinality();
	}

	public double evaluate(Chromosome chrom) {
		int valueTotal = 0;
		for (int i = 7; i >= 0; i--) {
			IntegerGene gene = (IntegerGene) chrom.getGene(i);
			Integer value = (Integer) gene.getAllele();
			valueTotal += value;
			valueTotal <<= 4;
		}
		int correct = correctBits(valueTotal);
		// we return the square, to reward exact answers
		return correct * correct;
	}

	public static void main(String[] args) {
		Configuration config = new DefaultConfiguration();
		// eight genes
		Gene[] genes = new Gene[8];
		for (int i = 0; i < 8; i++) {
			// each one a 4-bit integer (0-15 inclusive)
			genes[i] = new IntegerGene(0, 15);
		}
		Chromosome sample = new Chromosome(genes);
		try {
			config.setSampleChromosome(sample);
			DemuxFitness fitTest = new DemuxFitness();
			config.setFitnessFunction(fitTest);
			config.setPopulationSize(200);
			Genotype population = Genotype.randomInitialGenotype(config);
			for (int i = 0; i < 1000; i++) {
				population.evolve();
			}
			Chromosome fittest = population.getFittestChromosome();
		} catch (InvalidConfigurationException e) {
			e.printStackTrace();
		}
	}
}