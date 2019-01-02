/*
 * Naman Gangwani
 * nkg160030
 * Project.java
 */

import java.util.*;
import java.io.*;

public class Project1 {
	
	public static void main(String[] args) throws IOException
	{
		/*
		 * Validates command line input
		 * Makes sure file exists
		 * Checks to see if timer interrupt value is an integer
		 */
		if (args.length >= 2)
		{
			// Checks to see if the file is valid
			try
			{
				Scanner testFile = new Scanner(new File(args[0]));
				testFile.close();
			} catch (Exception FileNotFoundException)
			{
				System.err.println("ERROR: "+args[0]+" is not a valid file.");
				System.exit(1);
			}
			
			// Checks to see if the file is valid
			try
			{
				int interr = Integer.parseInt(args[1]);
			} catch (Exception e)
			{
				System.err.println("ERROR: "+args[1]+" is not an integer. The timer value must be an integer.");
				System.exit(1);
			}
		}
		else
		{
			System.err.println("ERROR: Only "+args.length+" argument(s) provided. Require at least 2.");
			System.exit(1);
		}
   
		// Retrieves command line input
		String inputFile = args[0];
		int timerInterrupt = Integer.parseInt(args[1]);
		
		Runtime rt = Runtime.getRuntime();
		try
		{
			rt.exec("javac Memory.java"); // Compiles the memory program
			Process memory = rt.exec("java Memory "+inputFile); // Creates a new process for managing the memory
			
			// Creates and runs a new CPU process to go alongside the newly created memory process
			CPU cpu = new CPU(memory, timerInterrupt);
			cpu.startProcess();
		}
		catch (Throwable t)
		{
			System.err.println("New process creation failed.");
			t.printStackTrace();
		}
	}
	
	/* 
	 * Emulates the CPU, its registers, its stack, and its communication with the memory process through pipes
	 * Class is private to ensure that it is only connected with the memory through pipes and nothing else 
	 */
	private static class CPU
	{
		private int PC, SP, IR, AC, X, Y; // Registers
		private Scanner fromMemory; // Scanner that reads input stream from memory
		private PrintWriter toMemory; // PrintWriter to write requests to memory
		private boolean kernelMode; // Keeps track of whether the CPU is in kernel mode or not
		private int timerInterrupt, timer; // Timer values for interrupts and handling
		
		/* 
		 * Constructor
		 * Initializes registers to their appropriate values
		 * CPU is given its memory process to communicate with and a timer interrupt value
		 */
		public CPU(Process memory, int timerInterrupt)
		{
			SP = 1000;
			this.fromMemory = new Scanner(memory.getInputStream());
			this.toMemory = new PrintWriter(memory.getOutputStream());
			this.timerInterrupt = timerInterrupt;
		}
		
		/*
		 * Starts the fetch-execute cycle of the CPU
		 * Continues until the user program tells it to stop
		 */
		public void startProcess()
		{
			while (IR != 50) // While the user program has not ended
			{
				incrementPCAndUpdateIR(); // Update PC and IR registers
				executeCurrentIR(); // Execute the current instruction
				timer++; // Increments timer
				if (timer >= timerInterrupt && !kernelMode && IR != 50) // Check for timer interrupt
				{
					// Enters kernel mode
					enterKernelMode();
					timer = 0;
					PC = 1000;
				}
			}
		}
		
		/* Pushes the given data into the stack held by the memory process */
		private void push(int data)
		{
			writeToMemory(--SP, data);
		}
		
		/* Pops the and returns the item at the bottom of the stack */
		private int pop()
		{
			return readFromMemory(SP++);
		}
		
		/* Pipes to the memory process and requests it to write data at a given address */
		private void writeToMemory(int addr, int data)
		{
			toMemory.println("write "+addr+" "+data);
			toMemory.flush();
		}
		
		/* 
		 * Pipes to the memory process and requests it to return data at a given address
		 * Assures that it is not attempting to access system adresses while in user mode
		*/
		private int readFromMemory(int addr)
		{
			if (!kernelMode && addr > 999) // If it is user mode and trying to access system memory
			{
				// End the memory process and exit
				toMemory.println("end");
				System.err.println("Memory violation: accessing system address "+addr+" in user mode.");
				System.exit(1);
			}
			
			toMemory.println("read "+addr); // Request memory process to give it the data at the address
			toMemory.flush();
			return Integer.parseInt(fromMemory.nextLine()); // Return what the memory gave it
		}
		
		/* 
		 * Saves all registers onto the system stack for interrupt handling and enables kernel mode
		 * User stack is no longer active
		 */
		private void enterKernelMode()
		{
			kernelMode = true;
			
			int oldSP = SP;
			SP = 2000;

			push(oldSP);
			push(PC);
			push(IR);
			push(AC);
			push(X);
			push(Y);
		}
		
		/* 
		 * Retrieves all registers values from the system stack for interrupt handling and disables kernel mode
		 * User stack is once again active 
		 */
		private void exitKernelMode()
		{
			Y = pop();
			X = pop();
			AC = pop();
			IR = pop();
			PC = pop();
			SP = pop();
			
			kernelMode = false;
		}
		
		/* 
		 * Increments the PC Register
		 * Updates the IR register in accordance with the PC register
		 */
		private void incrementPCAndUpdateIR()
		{
			IR = readFromMemory(PC++);
		}
		
		/* Executes the current instruction based on the defined set of the instruction set */
		private void executeCurrentIR()
		{
			switch(IR)
			{
				// Load(value): Load the value into the AC
				case 1:
					incrementPCAndUpdateIR();
					AC = IR;
					break;
				// Load(addr): Load the value at the address into the AC
				case 2:
					incrementPCAndUpdateIR();
					AC = readFromMemory(IR);
					break;
				// LoadIdx(addr): Load the value from the address found in the given address into the AC
				case 3:
					incrementPCAndUpdateIR();
					AC = readFromMemory(readFromMemory(IR));
					break;
				// LoadIdxX(addr): Load the value at (addr+X) into the AC
				case 4:
					incrementPCAndUpdateIR();
					AC = readFromMemory(IR + X);
					break;
				// LoadIdxY(addr): Load the value at (addr+Y) into the AC
				case 5:
					incrementPCAndUpdateIR();
					AC = readFromMemory(IR + Y);
					break;
				// LoadSpX: Load from (SP + X) into the AC
				case 6:
					AC = readFromMemory(SP + X);
					break;
				// Store(addr): Store the value in the AC into the address
				case 7:
					incrementPCAndUpdateIR();
					writeToMemory(IR, AC);
					break;
				// Get: Gets a random int from 1 to 100 into the AC
				case 8:
					AC = (int) (Math.random() * 101);
					break;
				/* Put port: If port = 1, writes AC as an int to the screen
							 If port = 2, writes AC	as a char to the screen	*/
				case 9:
					incrementPCAndUpdateIR();
					if (IR == 1)
						System.out.print(AC);
					else if (IR == 2)
						System.out.print((char) AC);
					break;
				// AddX: Add the value in X to the AC
				case 10:
					AC+=X;
					break;
				// AddY: Add the value in Y to the AC
				case 11:
					AC+=Y;
					break;
				// SubX: Subtract the value in X from the AC
				case 12:
					AC-=X;
					break;
				// SubY: Subtract the value in Y from the AC
				case 13:
					AC-=Y;
					break;
				// CopyToX: Copy the value in the AC to X
				case 14:
					X = AC;
					break;
				// CopyFromX: Copy the value in X to the AC
				case 15:
					AC = X;
					break;
				// CopyToY: Copy the value in the AC to Y
				case 16:
					Y = AC;
					break;
				// CopyFromY: Copy the value in Y to the AC
				case 17:
					AC = Y;
					break;
				// CopyToSp: Copy the value in AC to the SP
				case 18:
					SP = AC;
					break;
				// CopyFromSp: Copy the value in SP to the AC
				case 19:
					AC = SP;
					break;
				// Jump(addr): Jump to the address
				case 20:
					incrementPCAndUpdateIR();
					PC = IR;
					break;
				// JumpIfEqual(addr): Jump to the address only if the value in the AC is zero
				case 21:
					incrementPCAndUpdateIR();
					if (AC == 0)
						PC = IR;
					break;
				// JumpIfNotEqual(addr): Jump to the address only if the value in the AC is not zero
				case 22:
					incrementPCAndUpdateIR();
					if (AC != 0)
						PC = IR;
					break;
				// Call(addr): Push return address onto stack, jump to the address
				case 23:
					incrementPCAndUpdateIR();
					push(PC);
					PC = IR;
					break;
				// Ret: Pop return address from the stack, jump to the address
				case 24:
					PC = pop();
					break;
				// IncX: Increment the value in X
				case 25:
					X++;
					break;
				// DecX: Decrement the value in X
				case 26:
					X--;
					break;
				// Push: Push AC onto stack
				case 27:
					push(AC);
					break;
				// Pop: Pop from stack into AC
				case 28:
					AC = pop();
					break;
				// Int: perform system call
				case 29:
					if (!kernelMode)
					{
						enterKernelMode();
						PC = 1500;
					}
					break;
				// IRet: Return from system call
				case 30:
					if (kernelMode)
						exitKernelMode();
					break;
				// End: End execution
				case 50:
					toMemory.println("end");
					break;
				// If it is none of the valid instructions
				default:
					toMemory.println("end");
					System.err.println("ERROR: "+IR+" is not valid instruction.");
					break;
			}
		}
	}
}