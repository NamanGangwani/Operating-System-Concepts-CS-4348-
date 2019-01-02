/*
 * Naman Gangwani
 * nkg160030
 * Memory.java
 */

import java.io.*;
import java.util.*;

public class Memory {
	public static int[] memory; // Memory representation (0-999 = user program; 1000-1999 = system code)
	
	public static void main(String[] args) throws FileNotFoundException
	{
		memory = new int[2000]; // Initializes memory
		Scanner file = new Scanner(new File(args[0]));
		
		/* Loads user program and system codes into memory */
		int i = 0;
		while (file.hasNextLine()) // While there are instructions to read
		{
			String line = file.nextLine().trim(); // Read line
			if (!line.equals("")) // If it's not an empty line
			{
				String first = line.split("\\s+")[0]; // Retrieve the first word in the line
		        char firstChar = first.charAt(0); // Retrieve the first character of the first word
				if (firstChar != '.') // If it is a "jump-to" instruction line
		        {
					if ((int) firstChar >= 48 && (int) firstChar <= 57) // If it is a line with an instruction
					{
						// Save the instruction into memory
						int val = Integer.parseInt(first);
						memory[i++] = val;
					}
		        }
				else
					i = Integer.parseInt(first.substring(1)); // Change the memory pointer location
			}
		}
		file.close();
		
		/*
		 * Pipe Control
		 * Handles input stream when the CPU tries to communicate with the memory process
		 */
		Scanner input = new Scanner(System.in);
		while (input.hasNextLine()) // While this process is running
		{
			String[] commandLineInput = input.nextLine().split("\\s+");
			
			if (commandLineInput[0].equals("read")) // If the CPU wants to read from memory
				// Send the value of its request to memory location back
				System.out.println(read(Integer.parseInt(commandLineInput[1])));
			else if (commandLineInput[0].equals("write")) // If the CPU wants to write to memory
				 // Write the data to the memory address both specified by the CPU
				write(Integer.parseInt(commandLineInput[1]), Integer.parseInt(commandLineInput[2]));
			else
				break; // Otherwise, the memory process must end
		}
		input.close();
	}
	
	/* Returns the data at a specified memory address  */
	private static int read(int addr)
	{
		return memory[addr];
	}
	
	/* Writes the value at a specified memory address  */
	private static void write(int addr, int data)
	{
		memory[addr] = data;
	}
}
