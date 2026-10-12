(define (reciprocal n)
  (if (= n 0)
         (= n -1)
         (/ 1 n)
  )
)

(define (square n)
  (* n n)
)

(define (list_sumr mylist)
  (if (null? mylist)
      0
      (+ (car mylist) (list_sumr (cdr mylist)))
      )
  )

(define (list_copy mylist)
  (if (null? mylist)
      '()
      (cons (car mylist) (list_copy (cdr mylist)))
      )
  )

(define (x1 mylist)
  (if (null? mylist)
      '(abc)
      '123
      )
  )

(define (x2 mylist)
  (if (null? mylist)
      'abc
      'cons
      )
  )

(define (x3 mylist)
  (if (null? mylist)
      'if
      'display
      )
  )

(define (x4 mylist)
  (if (null? mylist)
      'newline
      'car
      )
  )

(define (x5 mylist)
  (if (null? mylist)
      'and
      'or
      )
  )

(define (x6 mylist)
  (if (null? mylist)
      'not
      'define
      )
  )

(define (x7 mylist)
  (if (null? mylist)
      '+
      '-
      )
  )

(define (x8 mylist)
  (if (null? mylist)
      '/
      '*
      )
  )

(define (x9 mylist)
  (if (null? mylist)
      '=
      '>
      )
  )

(define (x10 mylist)
  (if (null? mylist)
      '<
      '>=
      )
  )

(define (x11 mylist)
  (if (null? mylist)
      '<=
      ''
      )
  )

(define (x12 mylist)
  (if (null? mylist)
      'ls
      '0
      )
  )

(define (x13 mylist)
  (if (null? mylist)
      '5
      '%
      )
  )

(define (x14 mylist)
  (if (null? mylist)
      'c
      'string
      )
  )


(define (odd_copy mylist)
  (if (null? mylist)
      '()
      (if (null? (cdr mylist))
	  (cons (car mylist) '())
	  (cons (car mylist) (odd_copy (cddr mylist)))
	  )
      )
  )

(define (even_copy mylist)
  (if (null? mylist)
      '()
      (if (null? (cdr mylist))
	  '()
	  (cons (cadr mylist) (even_copy (cddr mylist)))
      )
  )
)

(define (last mylist)
  (if (null? mylist)
      '()
      (if (null? (cdr mylist))
	  (cons (car mylist) '())
	  (last (cdr mylist))
	  )
      )
  )

(define (insert_last myvalue mylist)
  (if (null? mylist)
      (cons myvalue '())
      (cons (car mylist) (insert_last myvalue (cdr mylist)))
      )
  )

(define (remove_last mylist)
  (if (null? mylist)
      '()
      (if (null? (cdr mylist))
	  '()
	  (cons (car mylist) (remove_last (cdr mylist)))
	  )
      )
  )

(define (list_reverse mylist)
  (if (null? mylist)
      '()
      (reverse mylist)
      )
  )
