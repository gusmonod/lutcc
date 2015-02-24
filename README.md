lutcc
=====

Lutin compiler. Complete description of the language in [`grammar.md`](./grammar.md).

Example lutin code
------------------

	var x;
	const n = 10, n2 = 100;

	ecrire n + n2;
	x := n + n2;
	ecrire x + 2;
	lire x;
	ecrire x;

This program declares one variable and two constants, writes `110` to the standard output, affects a value to `x` which is then written, and finally, it writes the user input on the standard output.

Notes:
------

+ Declarations (`var` and `const`) **must** be done **before** any instruction.
+ `ecrire` means write to standard output, and `lire` means read from standard input.
+ The language only supports **positive integers**
+ Constants **must** be initialized with a **positive integer litteral**
+ Variables **may not** be initialized