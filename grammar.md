Grammar of lutcc
================

Terminal symbols
----------------

Keywords:
+ `con` = const
+ `var` = var
+ `ecr` = ecrire
+ `lir` = lire

Operators:
+ `aff` = affectation (/^:=/)
+ `plu` = `+`
+ `min` = `-`
+ `mul` = `*`
+ `div` = `/`
+ `opp` = `(`
+ `com` = `,`
+ `clo` = `)`
+ `col` = `;`
+ `equ` = `=`

Operands:
+ `idv` = name of a variable
+ `num` = number (positive integer)

Non-terminal symbols
--------------------

+ `A ` = Axiom
+ `P ` = Program
+ `Ld` = List of declarations (may be empty)
+ `Li` = List of instructions (at least one)
+ `D ` = Declaration
+ `Lv` = List of variables (may be empty)
+ `Lc` = List of constants (may be empty)
+ `I ` = Instruction
+ `E ` = Expression

Grammar rules
-------------

1.  `A  -> P`

2.  `P  -> Ld Li`
3.  `Ld -> Ld D`
4.  `Ld -> ε`
5.  `Li -> Li I`
6.  `Li -> I`

7.  `D  -> var idv Lv ;`
8.  `D  -> con idv = num Lc ;`
9.  `Lv -> Lv , idv`
10. `Lv -> ε`
11. `Lc -> , idv = num`
12. `Lc -> ε`

13. `I -> idv := E ;`
14. `I -> ecr E ;`
15. `I -> lir idv ;`

16. `E  -> E + E`
17. `E  -> E - E`
18. `E  -> E * E`
19. `E  -> E / E`
20. `E  -> (E)`
21. `E  -> idv`
22. `E  -> num`
