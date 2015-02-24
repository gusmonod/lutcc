Grammar of lutcc
================

Terminal symbols
----------------

Keywords:
+ `co` = const
+ `va` = var
+ `ec` = ecrire
+ `li` = lire 

Operators:
+ `af` = affectation (/^:=/)
+ `op` = `+`, `-`, `*`, `/`

Operands:
+ `id` = name of a variable
+ `nu` = number (positive integer)

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

7.  `D  -> va id Lv ;`
8.  `D  -> co id = nu Lc ;`
9.  `Lv -> Lv , id`
10. `Lv -> ε`
11. `Lc -> , id = nu`
12. `Lc -> ε`

13. `I -> id af E ;`
14. `I -> ec E ;`
15. `I -> li id ;`

16. `E  -> E + E`
17. `E  -> E - E`
18. `E  -> E * E`
19. `E  -> E / E`
20. `E  -> (E)`
21. `E  -> id`
22. `E  -> nu`
