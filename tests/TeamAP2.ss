; Assignment: Project 2 Testing
; Authors: Nazania Barraza, Scott Mitchel, & JT Palmer
; Date: Monday, April 23, 2017
; Description: A testing file that will make sure our project 2 works correctly for every token type/grammer rule.


(define (equalities a b c d)
  (if (and (= a b) (= c d)) ; Checking AND_T and rule 22.
      (display 'both_equal)
      (if (or (= a b) (= c d)) ; Checking OR_T and rule 23.
	  (display 'one_equal)
	  (display 'no_equal)
	  )
      )
  (newline)
  )

(define (checkInput a)
  (if (number? a) ; Checking NUMBERP_T and rule 25.
      (if (zero? a) ; Checking ZEROP_T and rule 28.
	  (display 'is_zero)
	  (display 'is_number)
	  )
      (if (string? a) ; Checking STRINGP_T and rule 31.
	  (display 'is_string)
	  (if (symbol? a) ; Checking SYMBOLP_T and rule 26.
	      (display 'is_symbol)
	      (if (char? a) ; Checking CHARP_T and rule 30.
		  (display 'is_character)
		  )
	      )
	  )
      )
  (newline)
  )

(define (quantities a b)
  (if (<= a b) ; Checking LTE_T and rule 40.
      (if (< a b) ; Checking LT_T and rule 38.
	  (display 'a<b)
	  (display 'a<=b)
	  )
      (if (>= a b) ; Checking GTE_T and rule 39.
	  (if (> a b) ; Checking GT_T and rule 37.
	      (display 'a>b)
	      (display 'a>=b)
	      )
	  )
      )
  (newline)
  )

(define (isCoolLetter n)
  (if (= n 'n) ; Checking rule 18.
      (display 'Very_Cool_Letter)
      )
  (newline)
  )

(define (checkAnyOtherTokens)
  (display 'cons) ; Checking rule 47.
  (newline)
  (display 'if) ; Checking rule 48.
  (newline)
  (display 'display) ; Checking rule 49.
  (newline)
  (display 'newline) ; Checking rule 50.
  (newline)
  (display 'car) ; Checking rule 51.
  (newline)
  (display 'and) ; Checking rule 52.
  (newline)
  (display 'or) ; Checking rule 53.
  (newline)
  (display 'not) ; Checking rule 54.
  (newline)
  (display 'define) ; Checking rule 55.
  (newline)
  (display 'number?) ; Checking rule 56.
  (newline)
  (display 'symbol?) ; Checking rule 57.
  (newline)
  (display 'list?) ; Checking rule 58.
  (newline)
  (display 'zero?) ; Checking rule 59.
  (newline)
  (display 'null?) ; Checking rule 60.
  (newline)
  (display 'char?) ; Checking rule 61.
  (newline)
  (display 'string?) ; Checking rule 62.
  (newline)
  (display '+) ; Checking rule 63.
  (newline)
  (display '-) ; Checking rule 64.
  (newline)
  (display '/) ; Checking rule 65.
  (newline)
  (display '*) ; Checking rule 66.
  (newline)
  (display '=) ; Checking rule 67.
  (newline)
  (display '>) ; Checking rule 68.
  (newline)
  (display '<) ; Checking rule 69.
  (newline)
  (display '>=) ; Checking rule 70.
  (newline)
  (display '<=) ; Checking rule 71.
  (newline)
  (display 'naz_is_amazing) ; Checking rule 72.
  (newline)
  )
