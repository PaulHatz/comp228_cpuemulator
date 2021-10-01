#include <iostream>
/*
loop:
  fld  f2,4(r3)               // program to be
  fld  f4,8(r5)               // simulated;
  fmul f6,f2,f4               // no pipelining
  fadd f8,f4,f6
  fst  f6,8(r5)
  fst  f8,4(r3)
  fld  f2,8(r7)
  fst  f2,8(r7)
  subi r9,r9,1
  bne  r9,r1,loop             // r1 is permanently 0
*/

//For testing only
/*
int *reversedAsm()
{
	int Reg[10] = { 0 };
	Reg[1] = 0;
	Reg[9] = 2;
	Reg[3] = 303;
	Reg[5] = 505;
	Reg[7] = 707;

	int *Mem = new int[800]();

	Mem[0] = 324; Mem[1] = 548; Mem[2] = 246; // their ten argument
	Mem[3] = 468; Mem[4] = 568; Mem[5] = 384; // lists
	Mem[6] = 728; Mem[7] = 728; Mem[8] = 991;
	Mem[9] = 910;
	
	Mem[307] = 41; Mem[513] = 23;             // initial memory values
	Mem[715] = 37;

	do {
		Reg[2] = Mem[4 + Reg[3]];	// fld  f2,4(r3)
		Reg[4] = Mem[8 + Reg[5]];	// fld  f4,8(r5)
		Reg[6] = Reg[2] * Reg[4];	// fmul f6,f2,f4
		Reg[8] = Reg[4] + Reg[6];	// fadd f8,f4,f6
		Mem[Reg[5] + 8] = Reg[6];	// fst  f6,8(r5)
		Mem[Reg[3] + 4] = Reg[8];	// fst  f8,4(r3)
		Reg[2] = Mem[Reg[7] + 8];	// fld  f2,8(r7)
		Mem[Reg[7] + 8] = Reg[2];	// fst  f2,8(r7)
		Reg[9] -= 1;				// subi r9,r9,1
	} while (Reg[9] != Reg[1]);

	std::cout << "Reversed Version:" << std::endl;

	for (int i = 0; i < 10; i++) {
		printf("Reg[%i]:  %X\n", i, Reg[i]);
	}

	return Mem;
}

void verifyResult(int *Mem)
{
	int *Mem2 = reversedAsm();

	if (Mem2) {
		size_t mm_count = 0;
		for (size_t i = 0; i < 800; i++) {
			if (Mem[i] != Mem2[i]) {
				printf("Error, mismatch at %u\n", i);
				mm_count++;
			}
		}

		delete Mem2;

		printf("Verification complete. Mismatch Count: %u\n", mm_count);
	}
}
*/

int main()
{
	// register file                          // ten RF registers:
	int Reg[10] = { 0 };                      // five 'f_reg' & five 'r_reg'
	Reg[1] = 0; Reg[9] = 2;                   // initial r-register values
	Reg[3] = 303; Reg[5] = 505; Reg[7] = 707;

	// main memory
	int  Mem[800] = { 0 };
	char XMem[] = "llmasslsub";               // ten opcodes, and
	Mem[0] = 324; Mem[1] = 548; Mem[2] = 246; // their ten argument
	Mem[3] = 468; Mem[4] = 568; Mem[5] = 384; // lists
	Mem[6] = 728; Mem[7] = 728; Mem[8] = 991;
	Mem[9] = 910;

	Mem[307] = 41; Mem[513] = 23;             // initial memory values
	Mem[715] = 37;

	bool branch_taken = true;				  // to leave outer loop


	for (int i = 0; i < 4; i++) {

		if (branch_taken) {
			std::cout << "Iteration " << i + 1 << std::endl;

			for (int pc = 0; pc < 10; pc++) {

				std::cout << "\n";                        // blank line


				//Start f-box
				char head = XMem[pc]; //opcode
				int tail = Mem[pc]; //Args

				std::cout << "f: Fetched instruction: " << head << "|" << tail << std::endl;

				//End f-box

				//Start d-box
				char op = head;

				std::cout << "d: Set opc to \'" << op << "\'" << std::endl;

				int arg3 = tail % 10;
				tail /= 10;
				int arg2 = tail % 10;
				tail /= 10;
				int arg1 = tail;

				int D_Out1 = 0, D_Out2 = 0, dreg = 0, sval = 0;

				switch (op)
				{

				case 'a': //fadd
				case 'm': //fmul
					D_Out1 = Reg[arg1];
					std::cout << "d: Set D_Out1 to \'" << D_Out1 << "\'" << std::endl;

					D_Out2 = Reg[arg2];
					std::cout << "d: Set D_Out2 to \'" << D_Out2 << "\'" << std::endl;

					dreg = arg3;
					std::cout << "d: Set dreg to f" << dreg << std::endl;

					break;

				case 'u': //subi

					D_Out1 = Reg[arg2]; //Value to subtract from
					std::cout << "d: Set D_Out1 to \'" << D_Out1 << "\'" << std::endl;

					D_Out2 = arg3; //Value to subtract
					std::cout << "d: Set D_Out2 to \'" << D_Out2 << "\'" << std::endl;

					dreg = arg1; //Register to store result
					std::cout << "d: Set dreg to r" << dreg << std::endl;
					break;

				case 'b': //bne

					D_Out1 = Reg[arg1];
					D_Out2 = Reg[arg2];

					branch_taken = D_Out1 == D_Out2 ? false : true;

					std::cout << "d: Set branch_taken to " << (branch_taken ? "true" : "false") << std::endl;

					break;

				case 's': //fst

					D_Out1 = arg3; //Offset to be added to write
					std::cout << "d: Set D_Out1 to " << D_Out1 << std::endl;

					D_Out2 = Reg[arg1]; //Register containing write address
					std::cout << "d: Set D_Out2 to " << D_Out2 << std::endl;

					sval = Reg[arg2]; //Value to store
					std::cout << "d: Set sval to " << sval << std::endl;

					break;

				case 'l': //fld

					D_Out1 = Reg[arg1]; //Address
					std::cout << "d: Set D_Out1 to \'" << D_Out1 << "\'" << std::endl;

					D_Out2 = arg3; //Offset
					std::cout << "d: Set D_Out2 to \'" << D_Out2 << "\'" << std::endl;

					dreg = arg2; //Dest
					std::cout << "d: Set dreg to f" << dreg << std::endl;

					break;

				default:
					std::cout << "d: Did nothing." << std::endl;
					break;
				}

				//End d-box

				//Start x-box
				int X_Out = 0;
				switch (op) {

					//Memory Reference Instructions

				case 'l': //fld
					X_Out = D_Out1 + D_Out2;
					std::cout << "x: Set X_Out to \'" << X_Out << "\'" << std::endl;
					break;

				case 's': //fst
					X_Out = D_Out2 + D_Out1;
					std::cout << "x: Set X_Out to " << X_Out << std::endl;
					break;
					//Register Reference Instructions
				case 'm': //fmul
					X_Out = D_Out1 * D_Out2;
					std::cout << "x: Set X_Out to " << X_Out << std::endl;

					break;
				case 'a': //fadd
					X_Out = D_Out1 + D_Out2;
					std::cout << "x: Set X_Out to " << X_Out << std::endl;
					break;

					//Register Immediate Instructions
				case 'u': //subi
					X_Out = D_Out1 - D_Out2;
					std::cout << "x: Set X_Out to " << X_Out << std::endl;
					break;


				default:
					std::cout << "x: Did nothing." << std::endl;
					break;
				}

				//End x-box

				//Start m-box
				int M_Out = 0;


				switch (op) {
				case 'l': //fld
					M_Out = Mem[X_Out];
					std::cout << "m: Set M_Out to \'" << M_Out << "\'" << std::endl;
					break;

				case 's': //fst
					if (X_Out < 800) {
						Mem[X_Out] = sval;
						std::cout << "m: Set Mem[X_Out] to " << sval << std::endl;
					}
					else std::cout << "m: X_Out value " << X_Out << " is out of range. Skipping." << std::endl;
					break;

				default:
					std::cout << "m: Did nothing." << std::endl;
					break;
				}
				//End m-box

				//Start w-box
				switch (op) {
				case 'a':
				case 'u':
				case 'm':
					if (dreg < 10) {
						Reg[dreg] = X_Out;
						std::cout << "w: Set " << (dreg % 2 == 0 ? "f" : "r") << dreg << " to " << X_Out << std::endl;
					}
					else std::cout << "w: dreg value " << dreg << " will cause overflow. Skipping." << std::endl;
					break;
				case 'l':
					if (dreg < 10) {

						Reg[dreg] = M_Out;
						std::cout << "w: Set " << (dreg % 2 == 0 ? "f" : "r") << dreg << " to " << M_Out << std::endl;
					}
					else std::cout << "w: dreg value " << dreg << " will cause overflow. Skipping." << std::endl;
					break;

				default:
					std::cout << "w: Did nothing." << std::endl;
					break;
				}
				//End w-box
			}
		}

		else break;
	}

	//Verify
	//std::cout << "Simulated Version:" << std::endl;
 	//
	//for (int i = 0; i < 10; i++) {
	//	printf("Reg[%i]:  %X\n", i, Reg[i]);
	//}
	//
	//verifyResult(Mem);

	return 0;
}


/* OUTPUT
Iteration 1

f: Fetched instruction: l|324
d: Set opc to 'l'
d: Set D_Out1 to '303'
d: Set D_Out2 to '4'
d: Set dreg to f2
x: Set X_Out to '307'
m: Set M_Out to '41'
w: Set f2 to 41

f: Fetched instruction: l|548
d: Set opc to 'l'
d: Set D_Out1 to '505'
d: Set D_Out2 to '8'
d: Set dreg to f4
x: Set X_Out to '513'
m: Set M_Out to '23'
w: Set f4 to 23

f: Fetched instruction: m|246
d: Set opc to 'm'
d: Set D_Out1 to '41'
d: Set D_Out2 to '23'
d: Set dreg to f6
x: Set X_Out to 943
m: Did nothing.
w: Set f6 to 943

f: Fetched instruction: a|468
d: Set opc to 'a'
d: Set D_Out1 to '23'
d: Set D_Out2 to '943'
d: Set dreg to f8
x: Set X_Out to 966
m: Did nothing.
w: Set f8 to 966

f: Fetched instruction: s|568
d: Set opc to 's'
d: Set D_Out1 to 8
d: Set D_Out2 to 505
d: Set sval to 943
x: Set X_Out to 513
m: Set Mem[X_Out] to 943
w: Did nothing.

f: Fetched instruction: s|384
d: Set opc to 's'
d: Set D_Out1 to 4
d: Set D_Out2 to 303
d: Set sval to 966
x: Set X_Out to 307
m: Set Mem[X_Out] to 966
w: Did nothing.

f: Fetched instruction: l|728
d: Set opc to 'l'
d: Set D_Out1 to '707'
d: Set D_Out2 to '8'
d: Set dreg to f2
x: Set X_Out to '715'
m: Set M_Out to '37'
w: Set f2 to 37

f: Fetched instruction: s|728
d: Set opc to 's'
d: Set D_Out1 to 8
d: Set D_Out2 to 707
d: Set sval to 37
x: Set X_Out to 715
m: Set Mem[X_Out] to 37
w: Did nothing.

f: Fetched instruction: u|991
d: Set opc to 'u'
d: Set D_Out1 to '2'
d: Set D_Out2 to '1'
d: Set dreg to r9
x: Set X_Out to 1
m: Did nothing.
w: Set r9 to 1

f: Fetched instruction: b|910
d: Set opc to 'b'
d: Set branch_taken to true
x: Did nothing.
m: Did nothing.
w: Did nothing.
Iteration 2

f: Fetched instruction: l|324
d: Set opc to 'l'
d: Set D_Out1 to '303'
d: Set D_Out2 to '4'
d: Set dreg to f2
x: Set X_Out to '307'
m: Set M_Out to '966'
w: Set f2 to 966

f: Fetched instruction: l|548
d: Set opc to 'l'
d: Set D_Out1 to '505'
d: Set D_Out2 to '8'
d: Set dreg to f4
x: Set X_Out to '513'
m: Set M_Out to '943'
w: Set f4 to 943

f: Fetched instruction: m|246
d: Set opc to 'm'
d: Set D_Out1 to '966'
d: Set D_Out2 to '943'
d: Set dreg to f6
x: Set X_Out to 910938
m: Did nothing.
w: Set f6 to 910938

f: Fetched instruction: a|468
d: Set opc to 'a'
d: Set D_Out1 to '943'
d: Set D_Out2 to '910938'
d: Set dreg to f8
x: Set X_Out to 911881
m: Did nothing.
w: Set f8 to 911881

f: Fetched instruction: s|568
d: Set opc to 's'
d: Set D_Out1 to 8
d: Set D_Out2 to 505
d: Set sval to 910938
x: Set X_Out to 513
m: Set Mem[X_Out] to 910938
w: Did nothing.

f: Fetched instruction: s|384
d: Set opc to 's'
d: Set D_Out1 to 4
d: Set D_Out2 to 303
d: Set sval to 911881
x: Set X_Out to 307
m: Set Mem[X_Out] to 911881
w: Did nothing.

f: Fetched instruction: l|728
d: Set opc to 'l'
d: Set D_Out1 to '707'
d: Set D_Out2 to '8'
d: Set dreg to f2
x: Set X_Out to '715'
m: Set M_Out to '37'
w: Set f2 to 37

f: Fetched instruction: s|728
d: Set opc to 's'
d: Set D_Out1 to 8
d: Set D_Out2 to 707
d: Set sval to 37
x: Set X_Out to 715
m: Set Mem[X_Out] to 37
w: Did nothing.

f: Fetched instruction: u|991
d: Set opc to 'u'
d: Set D_Out1 to '1'
d: Set D_Out2 to '1'
d: Set dreg to r9
x: Set X_Out to 0
m: Did nothing.
w: Set r9 to 0

f: Fetched instruction: b|910
d: Set opc to 'b'
d: Set branch_taken to false
x: Did nothing.
m: Did nothing.
w: Did nothing.

*/

