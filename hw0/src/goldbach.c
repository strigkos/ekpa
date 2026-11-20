#include <ctype.h>   
/* Για την αναγνώριση ψηφίων και κενών χαρακτήρων με isdigit και isspace */

#include <stdbool.h>
/* Για τον τύπο bool και τιμές true και false για λογικούς ελέγχους */

#include <stdio.h>
/* Για ανάγνωση από stdin, εκτύπωση στο stdout και μηνύματα στο stderr */

#include <stdlib.h>
/* Για τη μετατροπή με strtoull και τους κωδικούς EXIT_SUCCESS και EXIT_FAILURE */

/* Δήλωση του μέγιστου αριθμού που επιτρέπεται */
const unsigned long long MAX_NUMBER = 10000000000;

/* Πινακάκι των καταστάσεων εισόδου */
enum input_status {
    INPUT_OK,           // 0
    INPUT_END,          // 1
    INPUT_INVALID,      // 2
    INPUT_OUT_OF_RANGE  // 3
};

/* Διαβάζει τον αριθμό ως μία λέξη με scanf και τη μετατρέπει με strtoull.
   Ελέγχει ότι διαβάστηκε ολόκληρη η λέξη και ότι η τιμή είναι εντός ορίου.
*/
enum input_status read_number(unsigned long long *number) {

    /* Πίνακας για την αποθήκευση της λέξης */
    char word[16];

    /* Δείκτης για το τέλος της λέξης */
    char *end;

    /* Διαβάζει έως 15 χαρακτήρες και αφήνει μία θέση για το '\0'. */
    if (scanf("%15s", word) != 1) {
        return INPUT_END;
    }

    /* Αν η λέξη συνεχίζεται μετά τους 15 χαρακτήρες, η είσοδος είναι άκυρη. */
    int next_char = getchar();
    if (next_char != EOF) {
        if (!isspace(next_char)) {
            return INPUT_INVALID;
        }
    }

    /* Ελέγχει αν ο αριθμός είναι αρνητικός */
    bool negative = false;
    const char *digits = word;
    if (word[0] == '-') {
        negative = true;
        digits = word + 1;
    }

    /* Ελέγχει αν τα ψηφία είναι έγκυρα */
    if (!isdigit((unsigned char)*digits)) {
        return INPUT_INVALID;
    }

    /* Έως 15 δεκαδικά ψηφία χωρούν σε unsigned long long. */
    unsigned long long value = strtoull(digits, &end, 10);
    if (*end != '\0') {
        return INPUT_INVALID;
    }

    /* Ένας αρνητικός ακέραιος είναι εκτός εύρους. Το -0 έχει τιμή 0. */
    if (negative && value != 0) {
        return INPUT_OUT_OF_RANGE;
    }

    if (value > MAX_NUMBER) {
        return INPUT_OUT_OF_RANGE;
    }

    *number = value;
    return INPUT_OK;
}

/* Ελέγχει αν ο n είναι πρώτος. 
   Πώς : Για τους περιττούς αριθμούς μεγαλύτερους του 2, δοκιμάζει περιττούς διαιρέτες μέχρι την τετραγωνική ρίζα του n 
*/
bool is_prime(unsigned long long n)
{
    if (n <= 1) {
        return false;
    }

    /* Το 2 είναι μία μοναδική περίπτωση : άρτιος & πρώτος */
    if (n == 2) {
        return true;
    }

    /* Για μεγαλύτερους άρτιους θα επιστρέψει false */
    if (n % 2 == 0) {
        return false;
    }

    /* Ψάχνω περιττούς διαιρέτες μέχρι την τετραγωνική ρίζα του n κ, εξετάζοντας το υπόλοιπο της διαίρεσης */
    for (unsigned long long divisor = 3; divisor * divisor <= n; divisor += 2) {
        if (n % divisor == 0) {
            return false;
        }
    }

    /* Αν δεν βρέθηκε κανένας διαιρέτης, τότε είναι πρώτος */
    return true;
}

/* Αναζήτηση για ζεύγος πρώτων
*/
void print_goldbach_pair(unsigned long long n)
{
    /* Η υπόθεση του θεωρήματος του Goldbach ισχύει μόνο για άρτιους αριθμούς μεγαλύτερους του 2, ή αλλιώς >= 4 */
    if (n <= 2 || n % 2 != 0) {
        puts("Goldbach's conjecture is applicable only to even numbers greater than 2!");
        return;
    }

    /* Το p θα είναι ο μικρότερος, πρώτος προσθετέος */
    unsigned long long p;

    /* Το 4 είναι μοναδική περίπτωση, με προσθετέους 2 + 2, όπου το είναι οι μοναδική περίπτωση άρτιου πρώτου.
        Για όλες τις άλλες περιπτώσεις πρέπει να ξεκινάμε από το 3 και να αναεβαίνουμε ανά 2, 
        ώστε να εξετάζουμε μόνο περιττούς αριθμούς. 
    */
    if (n == 4) {
        p = 2;
    } else {
        p = 3;
    }

    /* Δοκιμάζει τον πρώτο προσθετέο p σε αύξουσα σειρά έως το n / 2 */
    while (p <= n / 2) {
        unsigned long long q = n - p;
        if (is_prime(p) && is_prime(q)) {
            printf("%llu = %llu + %llu\n", n, p, q);
            return;
        }
        p += 2;
    }
    printf("No Goldbach pair found for %llu.\n", n);
}

/* Ζητά πρώτα τον αριθμό και έπειτα την επιλογή. 
    Εκτυπώνει το αποτέλεσμα και επαναλαμβάνει τη διαδικασία.
*/
int main(void)
{
    unsigned long long number;

    for (;;) {
        fputs("Give me a number: ", stdout);
        fflush(stdout);
        enum input_status status = read_number(&number);
        if (status == INPUT_END && !ferror(stdin)) {
            return EXIT_SUCCESS;
        }
        if (ferror(stdin) || status == INPUT_INVALID) {
            fputs("Invalid input: expected an integer (no more than 15 chars)\n", stderr);
            return EXIT_FAILURE;
        }
        if (status == INPUT_OUT_OF_RANGE) {
            fprintf(stderr, "Number should be in the range [0, %llu]\n", MAX_NUMBER);
            return EXIT_FAILURE;
        }

        printf("Choose how to test %llu - ('p' for prime, 'g' for goldbach) ", number);
        fflush(stdout);
        char choice;
        if (scanf(" %c", &choice) != 1 || (choice != 'p' && choice != 'g')) {
            fputs("Invalid choice: expected 'p' or 'g'\n", stderr);
            return EXIT_FAILURE;
        }
        int next_char = getchar();
        if (ferror(stdin) || (next_char != EOF && !isspace(next_char))) {
            fputs("Invalid choice: expected 'p' or 'g'\n", stderr);
            return EXIT_FAILURE;
        }

        if (choice == 'p') {
            printf("%llu is %sprime\n", number, is_prime(number) ? "" : "not ");
        } else {
            print_goldbach_pair(number);
        }
    }
}
