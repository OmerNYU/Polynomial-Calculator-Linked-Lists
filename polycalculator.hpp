//============================================================================
// Name         : Polynomial Calculator 
// Author       : Omer Hayat
// Version      : 1.1
// Date Created : 17-09-2025
// Date Modified: 18-09-2025 
// Description  : Polynomial Calculator in C++ using Singly Linked-List
//============================================================================

// I/O, file handling, strings, math helpers, stringstreams, char tests
#include<iostream>
#include<fstream>
#include<string>
#include<cmath>
#include<sstream>
#include<cctype>

// Use standard namespace for brevity in this assignment
using namespace std;

//============================== Node =========================================



// Represents a single polynomial term (coef * x^expo) in a singly linked list.
// Invariants:
//   - 'coef' is an int (zero terms are never stored by design).
//   - 'expo' is an int (list is kept in strict descending order of 'expo').
//   - 'next' points to the next term or nullptr.

class Node
{
	private:
		// Coefficient of the term
		int coef; 
		// Exponent of the term
		int expo; 
		// Next node in the singly linked list
		Node* next; 
		
	public:
		// Construct a node with (coef, expo); next starts as null
		Node(int coef, int expo) : coef(coef), expo(expo), next(nullptr)
		{}
		
		// Grant access to LinkedList and PolyCalculator internals
		friend class LinkedList;
		friend class PolyCalculator;
		
	public:
		/* Required for Auto-grader — Do not remove */
		friend void run_tests();
};

//=========================== LinkedList ======================================

// A minimal singly linked list that models a polynomial.
//
// Design guarantees:
//   1) Terms are stored in STRICTLY DESCENDING order by exponent.
//   2) There is at most ONE node per exponent (like terms are merged).
//   3) No node has coefficient 0 (zeros are pruned on insert/merge).
class LinkedList
{
	private:
		// Head pointer of the list (nullptr means empty polynomial)
		Node* head; 
		
	public:
		// Create an empty list
		LinkedList (); 
		
		// Insert (coef, expo) at the correct place, combining like terms
		void insert(int coef,int expo); 
		
		// Print the polynomial to a stream in canonical form
		void print(ostream& os=std::cout) const;
		
		// Delete all nodes and reset to empty
		void removeAll();	 
		
		// Structural equality (same terms in same order)
		bool operator==(const LinkedList &other);
		
		// Destructor ensures no memory leaks
		~LinkedList (); 
		
	public:
		// PolyCalculator needs access to head for operations
		friend class PolyCalculator;
		
		/* Required for Autograder — Do not remove */
		friend void run_tests();
	};

//========================== PolyCalculator ===================================

// Wrapper that owns Exp1 (list1), Exp2 (list2), and result (list3)
class PolyCalculator
{
	private:
		// Polynomial expressions
		LinkedList list1;
		LinkedList list2;
		LinkedList list3;
		
	public:
		// Read two expressions from keyboard into list1 and list2
		void input(); 
		
		// Parse a textual polynomial into a linked list (returns validity)
		bool parse(std::string expr,LinkedList& list);	
		
		// Display Exp1 and Exp2
		void display(std::ostream &os=std::cout); 
		
		// Compute list3 = list1 + list2 and print
		void add();  
		
		// Compute list3 = list2 - list1 and print  (matches main.cpp help)
		void sub();  
		
		// Compute list3 = list1 * list2 and print
		void mul(); 
		
		// Print p(x) then p(value) for the chosen expression (1 or 2)
		void evaluate(int expID, int x); 
		
		// Return the degree (max exponent) of the chosen expression
		int getDegree(int expID);	
		
		// Convenience: report whether Exp1 == Exp2
		void isEqual(); 
		
		// Load Exp1 and Exp2 from a file (first two lines)
		void read(std::string path); 
		
		// You can add more methods or helper functions if needed.
		
	public:
		/* Required for Autograder — Do not remove */
		friend void run_tests();
};

//======================================================
// Implementations
//======================================================

// Initialize an empty list
LinkedList::LinkedList(): head(nullptr) {}

// Inserts (coef, expo) while maintaining all polynomial invariants.
// Fast paths handled first (empty list / becomes new head / merge at head).
// The inner walk finds the correct "window" (prev, curr) for insertion.
// If the exponent already exists, coefficients are combined; a 0 sum deletes the node.
void LinkedList::insert(int coef, int expo) {
    // Skip inserting pure zero terms
    if (coef == 0) return;

    // If list is empty, new node becomes head
    if (head == nullptr) {
        head = new Node(coef, expo);
        return;
    }

    // If new exponent is larger than head's, insert at the front
    if (expo > head->expo) {
        Node* n = new Node(coef, expo);
        n->next = head;
        head = n;
        return;
    }

    // If exponent matches head, combine coefficients and drop if result becomes zero
    if (expo == head->expo) {
        head->coef += coef;
        if (head->coef == 0) {
            Node* tmp = head;
            head = head->next;
            delete tmp;
        }
        return;
    }

    // Otherwise, walk to the correct insertion spot
    Node* prev = head;
    Node* curr = head->next;
    while (curr != nullptr && curr->expo > expo) {
        prev = curr;
        curr = curr->next;
    }

    // If we found the same exponent, merge and possibly delete the node if sum is zero
    if (curr != nullptr && curr->expo == expo) {
        curr->coef += coef;
        if (curr->coef == 0) {
            prev->next = curr->next;
            delete curr;
        }
    } else {
        // Insert new node between prev and curr (or at tail)
        Node* n = new Node(coef, expo);
        prev->next = n;
        n->next = curr;
    }
}

// Prints the polynomial in canonical form, e.g.:
//   +4x^3 +2x^2 -6x^1 +8x^0
// Rules:
//   - Always show an explicit sign for each term.
//   - Print '0' if the list is empty.
void LinkedList::print(ostream &os) const {
    // Empty polynomial is printed as 0
    if (!head) { os << "0"; return; }

    // Print first term (include leading + if positive)
    const Node* curr = head;
    os << (curr->coef >= 0 ? "+" : "") << curr->coef << "x^" << curr->expo;
    curr = curr->next;

    // Print remaining terms, each preceded by a space and explicit sign
    while (curr) {
        os << ' ' << (curr->coef >= 0 ? "+" : "") << curr->coef << "x^" << curr->expo;
        curr = curr->next;
    }
}

// Structural equality: the two lists are identical iff they have
// the same sequence of (coef, expo) pairs and the same length.
bool LinkedList::operator==(const LinkedList &other) {
    Node* curr1 = this->head;
    Node* curr2 = other.head;

    // Walk both lists in lockstep
    while (curr1 != nullptr && curr2 !=nullptr) {
        // Mismatch in coef or expo ⇒ not equal
        if (curr1->coef != curr2->coef || curr1->expo != curr2->expo) return false;
        curr1 = curr1->next;
        curr2 = curr2->next;
    }

    // Both must end at the same time to be equal
    if (curr1 == nullptr && curr2 ==nullptr) return true;
    else return false;
}

// Deletes every node in the list (iterative to avoid recursion depth).
// After this call, 'head' is guaranteed to be nullptr (empty polynomial).
void LinkedList::removeAll() {
    if (head == nullptr) return;

    Node* curr = head;
    while (curr != nullptr) {
        Node* next = curr->next;
        delete curr;
        curr = next;
    }
    head = nullptr;
}

// Ensure cleanup at destruction
LinkedList::~LinkedList() {
    removeAll();
} 

//============================== I/O helpers ==================================

// Reads two raw lines from stdin, parses them into normalized polynomials,
// and echoes the normalized forms. Existing expressions are cleared first.
// Any parse failure leaves both lists cleared and prints an error.
void PolyCalculator::input() {
    list1.removeAll();
    list2.removeAll();

    string s1, s2;
    cout << "Enter Exp1: "; getline(cin, s1);
    cout << "Enter Exp2: "; getline(cin, s2);

    // Build Exp1; if invalid, report and abort
    if (!parse(s1, list1)) {
        cout << "Invalid expression for Exp1\n"; return;
    }
    // Build Exp2; if invalid, report and abort
    if (!parse(s2, list2)) {
        cout << "Invalid expression for Exp2\n"; return;
    }

    // Echo back normalized expressions
    cout << "Exp1: "; list1.print(cout); cout << '\n';
    cout << "Exp2: "; list2.print(cout); cout << '\n';
}

//=========================== Parse utilities =================================

// ---------------- Parsing helpers ----------------
// skipSpaces: advance index 'i' over ASCII spaces only.
// peek:       safe look-ahead; returns '\0' at end.
// take:       consume one char at 'i' and return it (or '\0' at end).
// parseInt:   parse a non-negative integer starting at s[i]; sets 'ok'.


// Skip spaces before/after tokens during parsing
void skipSpaces(const string &s, int &i) {
    while (i < (int)s.length() && s[i] == ' ') {
        i++;
    }
}

// Peek at current char or '\0' if at end
char peek(const string &s, int i) {
    if (i < (int)s.length()) return s[i];
    return '\0'; // sentinel for "nothing left"
}

// Consume and return current char (or '\0' if none)
char take(const string &s, int &i) {
    if (i >= (int)s.length()) return '\0'; 
    char c = s[i];
    i++;
    return c;
}

// Parse a non-negative integer at s[i]; sets ok=true on success and advances i
int parseInt(const string &s, int &i, bool &ok) {
    int val = 0;
    ok = false;  // assume failure until a digit is seen

    if (i >= (int)s.length() || !isdigit(s[i])) {
        return 0;  // no digits found
    }

    // Accumulate digits into an int value
    while (i < (int)s.length() && isdigit(s[i])) {
        val = val * 10 + (s[i] - '0');
        i++;
        ok = true;
    }

    return val;
}

//============================== Parse ========================================

/* Grammar accepted by parse():
   expr    := term { ('+' | '-') term }
   term    := integer 'x' '^' integer
   integer := digit { digit }
   Notes:
     - Spaces are allowed between tokens (and safely skipped).
     - We require explicit 'x^' for each term (no implicit exponents).
     - On any malformed token, we return false and clear 'list'.
*/
bool PolyCalculator::parse(std::string expr, LinkedList& list) {
    // Start from a clean list for this parse attempt
    list.removeAll();

    int i = 0;
    bool first = true;
    int terms = 0;

    while (true) {
        // Allow spaces before a term
        skipSpaces(expr, i);

        // End of string is OK only if we parsed at least one term
        if (i >= (int)expr.size()) break;

        // Parse an optional sign for the first term, and a required sign later
        int sign = +1;
        char ch = peek(expr, i);
        if (ch == '+' || ch == '-') {
            take(expr, i);
            sign = (ch == '+') ? +1 : -1;
        } else if (!first) {
            // Missing operator between terms ⇒ invalid
            list.removeAll();
            return false;
        }

        skipSpaces(expr, i);

        // Parse the (absolute) coefficient: at least one digit
        bool ok = false;
        int coefAbs = parseInt(expr, i, ok);
        if (!ok) { list.removeAll(); return false; }
        int coef = sign * coefAbs;

        skipSpaces(expr, i);

        // Expect literal 'x'
        if (peek(expr, i) != 'x') { list.removeAll(); return false; }
        take(expr, i);

        // Expect '^'
        if (peek(expr, i) != '^') { list.removeAll(); return false; }
        take(expr, i);

        skipSpaces(expr, i);

        // Parse the non-negative integer exponent
        ok = false;
        int expo = parseInt(expr, i, ok);
        if (!ok) { list.removeAll(); return false; }

        // Insert the validated term; insert() maintains invariants
        list.insert(coef, expo);
        terms++;
        first = false;

        // After a term, we either hit end or see the next sign
        skipSpaces(expr, i);
        if (i >= (int)expr.size()) break;

        char next = peek(expr, i);
        if (next == '+' || next == '-') {
            // Next loop will read this sign; do nothing here
            continue;
        } else {
            // Any other character between terms is invalid
            list.removeAll();
            return false;
        }
    }

    // At least one term is required
    if (terms == 0) { list.removeAll(); return false; }

    return true;
}

//============================= Operations ====================================

// list3 = list1 + list2
// Merge-walk both sorted lists by exponent (descending).
// Equal exponents: sum coefficients (drop if 0).
// Otherwise: carry the term with the larger exponent.
void PolyCalculator::add() {
    list3.removeAll();

    Node* p1 = list1.head;
    Node* p2 = list2.head;

    // Merge like a classic sorted-list merge
    while (p1 != nullptr && p2 != nullptr) {
        if (p1->expo == p2->expo) {
            int sum = p1->coef + p2->coef;
            if (sum != 0) list3.insert(sum, p1->expo);
            p1 = p1->next;
            p2 = p2->next;
        } else if (p1->expo > p2->expo) {
            // Carry the larger exponent from p1
            list3.insert(p1->coef, p1->expo);
            p1 = p1->next;
        } else {
            // Carry the larger exponent from p2
            list3.insert(p2->coef, p2->expo);
            p2 = p2->next;
        }
    }

    // Append any remaining terms
    while (p1 != nullptr) {
        list3.insert(p1->coef, p1->expo);
        p1 = p1->next;
    }
    while (p2 != nullptr) {
        list3.insert(p2->coef, p2->expo);
        p2 = p2->next;
    }

    // Print result in the format the grader expects
    cout << "Exp1 + Exp2 = ";
    list3.print(cout);
    cout << '\n';
}

// list3 = list2 - list1  (matches the "sub" label in main.cpp)
// Same merge-walk as add(), but:
//   - equal exponents: coef = p2->coef - p1->coef
//   - carry-only-from-list2 as-is, and from list1 as negated.
void PolyCalculator::sub() {
    list3.removeAll();
    Node* p1 = list1.head;  // Exp1
    Node* p2 = list2.head;  // Exp2

    // Compute list3 = Exp1 - Exp2
    while (p1 && p2) {
        if (p1->expo == p2->expo) {
            int diff = p1->coef - p2->coef;  // flipped order
            if (diff != 0) list3.insert(diff, p1->expo);
            p1 = p1->next; 
            p2 = p2->next;
        } else if (p1->expo > p2->expo) {
            // carry term from Exp1
            list3.insert(p1->coef, p1->expo);
            p1 = p1->next;
        } else { // p2->expo > p1->expo
            // carry negative of Exp2
            list3.insert(-p2->coef, p2->expo);
            p2 = p2->next;
        }
    }

    // drain leftovers
    while (p1) { list3.insert(p1->coef, p1->expo); p1 = p1->next; }
    while (p2) { list3.insert(-p2->coef, p2->expo); p2 = p2->next; }

    cout << "Exp1 - Exp2 = ";
    list3.print(cout);
    cout << '\n';
}


// Show both stored polynomials
void PolyCalculator::display(std::ostream &os) {
    os << "Exp1: ";
    list1.print(os);
    os << endl;

    os << "Exp2: ";
    list2.print(os);
    os << endl;
}

// list3 = list1 * list2
// Quadratic cross-product over terms; each product (c1*c2, e1+e2) is
// inserted into list3 where insert() merges equal exponents automatically.
void PolyCalculator::mul() {
    list3.removeAll();

    // Quadratic nested loop over terms; insert() combines exponents
    for (Node* p1 = list1.head; p1 != nullptr; p1 = p1->next) {
        for (Node* p2 = list2.head; p2 != nullptr; p2 = p2->next) {
            int newCoef = p1->coef * p2->coef;
            int newExpo = p1->expo + p2->expo;
            list3.insert(newCoef, newExpo);
        }
    }

    // Print result
    cout << "Exp1 * Exp2 = ";
    list3.print(cout);
    cout << '\n';
}

// Simple integer exponentiation used during evaluation
int intPow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

// Prints the selected polynomial as "p(x) = ...", then evaluates it at 'x'
// using intPow, and prints "p(x) = value". Stays in integer arithmetic.
void PolyCalculator::evaluate(int ExprID, int x) {
    // Choose which expression to evaluate (1 or 2)
    const LinkedList* target = nullptr;
    if (ExprID == 1)      target = &list1;
    else if (ExprID == 2) target = &list2;
    else { cout << "Error: Invalid ID" << endl; return; }

    // First print the polynomial text itself (matches screenshot spec)
    cout << "p(x) = ";
    target->print(cout);
    cout << '\n';

    // Then compute and print the numeric value
    int result = 0;
    for (Node* c = target->head; c; c = c->next) {
        result += c->coef * intPow(x, c->expo);
    }
    cout << "p(" << x << ") = " << result << '\n';
}

// Returns the maximum exponent of the requested polynomial.
// Because the list is maintained in descending order, head->expo is the degree.
// Returns -1 for invalid ID or empty polynomial.
int PolyCalculator::getDegree(int expID) {
    // Head has the largest exponent because we maintain descending order
    const Node* curr = (expID == 1 ? list1.head : expID == 2 ? list2.head : nullptr);
    if (expID != 1 && expID != 2) return -1;
    if (!curr) return -1;              // empty polynomial
    return curr->expo;
}

//============================== File I/O =====================================

// Loads two expressions from a text file (first two lines only).
// Behavior:
//   - Clears existing lists before reading.
//   - Trims a trailing '\r' (Windows CRLF) if present.
//   - On any failure (open/format), both lists are cleared and an error is printed.
//   - On success, no printing here; main() calls display() right after read.
void PolyCalculator::read(std::string path) {
    // Start fresh before loading
    list1.removeAll();
    list2.removeAll();

    // Try to open the file
    std::ifstream in(path);
    if (!in.is_open()) {
        std::cout << "Error: cannot open file \"" << path << "\"\n";
        return;
    }

    // Read the first two lines as Exp1 and Exp2
    std::string s1, s2;
    if (!std::getline(in, s1)) {
        std::cout << "Error: file does not contain Exp1\n";
        return;
    }
    if (!std::getline(in, s2)) {
        std::cout << "Error: file does not contain Exp2\n";
        return;
    }

    // Strip trailing '\r' for Windows-formatted files
    if (!s1.empty() && s1.back() == '\r') s1.pop_back();
    if (!s2.empty() && s2.back() == '\r') s2.pop_back();

    // Parse both; on failure, clear both lists to keep state consistent
    if (!parse(s1, list1)) {
        std::cout << "Error: invalid expression for Exp1 in file\n";
        list1.removeAll();
        list2.removeAll();
        return;
    }
    if (!parse(s2, list2)) {
        std::cout << "Error: invalid expression for Exp2 in file\n";
        list1.removeAll();
        list2.removeAll();
        return;
    }

    // Success: no printing here (main.cpp calls display() right after read)
}

// Convenience helper for manual equality check
void PolyCalculator::isEqual() {
    cout << (list1 == list2 ? "Equal\n" : "Not equal\n");
}
