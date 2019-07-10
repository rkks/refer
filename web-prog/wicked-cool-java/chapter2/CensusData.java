package com.wickedcooljava.strings;

import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import java.util.Scanner;

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
/*
 * This class loads data from the 1990 census statistical distribution files,
 * available from http://www.census.gov/genealogy/names/
 * <BR />
 * The program assumes the files are placed in either C:\projects\census
 * or /projects/census
 */

public class CensusData
{

    List boys = null;
    List girls = null;
    List surnames = null;
    
    public static void main(String[] args)
    throws IOException
    {
        CensusData census = new CensusData();
        Random r = new Random();
        List boys = census.getBoyNames();
        List girls = census.getGirlNames();
        List surnames = census.getSurnames();
        System.out.println("Loaded " + boys.size() + " boy names");
        System.out.println("Loaded " + girls.size() + " girl names");
        System.out.println("Loaded " + surnames.size() + " surnames");
        
        System.out.print("Random boy: ");
        String randomBoy = (String) boys.get(r.nextInt(boys.size()));
        System.out.println(randomBoy);
        System.out.print("Random girl: ");
        String randomGirl = (String) girls.get(r.nextInt(girls.size()));
        System.out.println(randomGirl);
        System.out.print("Random surname: ");
        String randomSurname = (String) surnames.get(r.nextInt(surnames.size())); 
        System.out.println(randomSurname);
    }
    
    public CensusData()
    throws IOException
    {
        // FYI: The Java API will auto-convert the / to \ for Windows
        boys = readNameFile("C:/projects/census/dist.male.first");
        girls = readNameFile("C:/projects/census/dist.female.first");
        surnames = readNameFile("C:/projects/census/dist.all.last");
    }
    
    private List readNameFile(String fileName)
    throws IOException
    {
        ArrayList list = new ArrayList();
        FileReader fileReader = new FileReader(fileName);
        // create a scanner from the data file
        Scanner scanner = new Scanner(fileReader);
        while (scanner.hasNext())
        {
            String name = scanner.next();
            float frequency = scanner.nextFloat();
            float cumulativeFrequency = scanner.nextFloat();
            int rank = scanner.nextInt();
            list.add(name);
        }
        scanner.close();        
        return list;
    }
    
    public List getBoyNames()
    {
        return boys;
    }
    
    public List getGirlNames()
    {
        return girls;
    }
    
    public List getSurnames()
    {
        return surnames;
    }
}
