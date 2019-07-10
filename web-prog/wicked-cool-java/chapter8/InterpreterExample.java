package com.wickedcooljava.misc;

import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Date;

import bsh.EvalError;
import bsh.Interpreter;

/*
 * Example code from Wicked Cool Java (No Starch Press) Copyright (C) 2005 Brian
 * D. Eubanks
 * 
 * This library is free software; you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation; either version 2.1 of the License, or (at your option)
 * any later version.
 * 
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * 
 * Note: The LGPL licence can be found online at http://www.gnu.org
 * 
 */
/**
 * Simple example of BeanShell interpreter
 */
public class InterpreterExample {
	public static void main(String[] args) {
		Interpreter shell = new Interpreter();
		try {
			shell.set("varName", "my data");
			shell.eval("date = new java.util.Date();");
			shell.eval("System.out.println(date);");
			shell.eval("System.out.println(varName);");
			shell.source("test.bsh");
			Date myCopy = (Date) shell.get("date");
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (EvalError e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
}