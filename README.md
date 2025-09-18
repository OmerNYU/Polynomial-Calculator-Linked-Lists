Polynomial Calculator

This project implements a Polynomial Calculator in C++ using Singly Linked Lists.
It was built as part of a Data Structures assignment to practice dynamic memory management, linked list operations, and polynomial algebra.

Features

Input & Parsing

Accepts polynomial expressions from keyboard or file.

Strict validation of format (+3x^2 -5x^1 +7x^0, etc.).

Automatically sorts terms by descending exponents, combines like terms, and removes zero coefficients.

Operations on Polynomials

Display: Prints the stored polynomials in canonical form.

Addition: Computes Exp1 + Exp2.

Subtraction: Computes Exp2 - Exp1 (per assignment specification).

Multiplication: Computes Exp1 * Exp2.

Evaluation: Prints the polynomial and its value at a given integer x.

Degree: Returns the degree (highest exponent) of a given polynomial.

Equality Check: Compares two polynomials structurally.

File I/O

Can read two polynomial expressions from a text file and load them into the linked lists.

Implementation Details

Data Structure:
Each polynomial is stored as a singly linked list, where each node contains a coefficient, exponent, and pointer to the next term.

Insertion:
Maintains sorted order by exponent and merges coefficients of identical terms.

Memory Management:
removeAll() and a destructor ensure no memory leaks.

Output Format:
Matches the assignment’s required style, e.g.

p(x) = +4x^3 +2x^2 +3x^1 +8x^0
p(2) = 54

Usage

Compile using g++ main.cpp -o polycalc.

Run ./polycalc.

Use commands like:

display

input

add

sub

mul

evaluate 1,3

getDegree 2

read filename.txt
