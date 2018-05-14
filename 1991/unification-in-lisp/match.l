
(in-package "USER")

(load "~nic/lisp/pattern/pattern.l")         ; this needs to be sorted!!
(load "~nic/lisp/unify/bindings-and-vars.l")


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  Printing...
;;
(defun pr (x) (terpri) (princ x))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  Structures to represent
;;  commutative and associative lists
;;
(defstruct com list)
(defstruct ass list)


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  MATCHING
;;   - with success and fail continuations
;;   - to implement associative and commutative lists
;;

(defun match (x p)
  (funcall (M x p)
	   ()
	   #'(lambda (e f)
	       (progn
		 (pr e)
		 (funcall f)))
	   #'(lambda () 'done)))


(defun andM (m1 m2)
  #'(lambda (e s f)
      (funcall m1 e #'(lambda (e f) (funcall m2 e s f)) f)))


(defun orM (m1 m2)
  #'(lambda (e s f)
      (funcall m1 e s #'(lambda () (funcall m2 e s f)))))


(defun fail ()
  #'(lambda (e s f)
      (declare (ignore e))
      (declare (ignore s))
      (funcall f)))


(defun succeed ()
  #'(lambda (e s f)
      (funcall s e f)))


(defun extend-if-possible (var val)
  #'(lambda (e s f)
      (let ((old-val (lookup var e)))
	(if (eq old-val 'no-binding)
	    (funcall s (extend var val e) f)
	  (funcall (M old-val val) e s f)))))



(defun M (x p)
  (pattern (x p)
     ((_ (var ?v))         (extend-if-possible v x))
     (((ass ?a) (ass ?b))  (assM a b nil))
     (((com ?a) (com ?b))  (comM nil a b))
     (((cons ?x1 ?x2)
       (cons ?p1 ?p2))     (andM (M x1 p1) (M x2 p2)))
     
     ((_ _)                (if (equalp x p)
			       (succeed)
			     (fail)))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  Match associative lists
;;  - matches lists so that each variable may be bound
;;    to a list segment, including null.
;;

(defun assM (x y a)
  (pattern (x y)
    ((nil nil)             (succeed))
    ((_   nil)             (fail))

    ((nil (cons ?yh ?yt))  (if (var-p yh)
			       (andM
				(M a yh)
				(assM nil yt nil))
			     (fail)))
    (((cons ?xh ?xt)
      (cons ?yh ?yt))      (if (var-p yh)
			       (orM
				(andM
				 (M a yh)
				 (assM x yt nil))
				(assM xt y (append a `(,xh))))
			     (andM
			      (M xh yh)
			      (assM xt yt nil))))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;  Match commutative lists
;;  - matches lists in any permutation
;;

(defun comM (a x y)
  (pattern (a x y)
    ((nil nil nil)       (succeed))
    ((_ nil _)           (fail))
    ((_ (cons ?xh ?xt)
	(cons ?yh ?yt))  (orM
			  (andM
			   (M xh yh)
			   (comM nil (append a xt) yt))
			  (comM (cons xh a) xt y)))
    ((_ _ _)   (fail))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   TRY
;;

(defun t3 ()
  (match
   `(,(make-ass :list '(M I I)) ,(make-ass :list '(M I I I I)))
   `(,(make-ass :list '(M ?x)) ,(make-ass :list '(M ?x ?x)))))

(defun t4 ()
  (match
   `(,(make-ass :list '(M I I I I)) ,(make-ass :list '(M U I)))
   `(,(make-ass :list '(?x I I I ?y)) ,(make-ass :list '(?x U ?y)))))

(defun t5 ()
  (match
   `(,(make-ass :list '(M I I I I)) ,(make-ass :list '(M I U)))
   `(,(make-ass :list '(?x I I I ?y)) ,(make-ass :list '(?x U ?y)))))


(defun t6 ()
  (match (make-ass :list '(1 2 3 4 5))
	 (make-ass :list '(?a ?b ?c))))


(defun t7 ()
  (match (make-com :list '(1 1 1 2 2 3 3))
	 (make-com :list '(1 2 ?a ?b ?c ?a ?b))))

(defun t8 ()
  (match (make-com :list '(1 2 3))
	 (make-com :list '(?a ?b ?c))))

(defun t9 ()
  (match (make-com :list '(1 2 3 4))
	 (make-com :list '(?a ?b ?c ?d))))
