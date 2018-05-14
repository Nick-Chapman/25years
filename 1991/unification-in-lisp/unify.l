
(in-package "USER")

(load "~nic/lisp/pattern/pattern.l")         ; this needs to be sorted!!
(load "~nic/lisp/unify/bindings-and-vars.l")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   UNIFICATION
;;

(defun unify (x y)
  (unify1 x y ()))


(defun-m unify1 (x y e)
  ((_ _ failed)         'failed)
  (((var _) _ _)        (extend-if-pos x y e))
  ((_ (var _) _)        (extend-if-pos y x e))
  (((cons ?t1 ?t2)
    (cons ?v1 ?v2) _)   (unify1 t1 v1 (unify1 t2 v2 e)))
  ((_ _ _)              (if (equalp x y) e 'failed)))
  

(defun extend-if-pos (var val env)
  (if (equiv var val env)
      env
    (let ((old-val (lookup var env)))
      (if (eq old-val 'no-binding)
	  (if (occurs var val env)
	      'failed
	    (extend var val env))
	(unify1 old-val val env)))))


(defun equiv (v x env)
  (pattern (x)
     ((var _)      (if (equalp v x) t (equiv v (lookup x env) env)))
     (_            nil)))


(defun occurs (v x env)
  (pattern (x)
    ((var _)       (if (equalp v x) t (occurs v (lookup x env) env)))
    ((cons ?a ?b)  (or (occurs v a env)
		       (occurs v b env)))
    (_             nil)))
   

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   MOST GENERAL UNIFIER
;;

(defun mgu (x y)
  (let ((b (unify x y)))
    (if (eq b 'failed)
	'failed
      (instantiate x b))))


(defun instantiate (x env)
  (pattern (x)
    ((var _)       (let ((v (lookup x env)))
		     (if (eq v 'no-binding)
			 x
		       (instantiate v env))))
    ((cons ?a ?b)  (cons (instantiate a env)
			 (instantiate b env)))
    (_              x)))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   Test...
;;

(defun t1 ()
  (unify  '(?x (1 ?y 3)) '((4 ?y 5) (1 (6 7) 3))))

(defun t2 ()
  (mgu '(?x (1 ?y 3)) '((4 ?y 5) (1 (6 7) 3))))

(defun t3 ()
  (unify '(?x ?x) '(?y (?y ?z))))  ; should fail

(defun t4 ()
  (unify '(?x ?y) '(?y ?x)))

(defun t5 ()
  (mgu '(?w ?x ?y ?z) '(?z ?y ?x ?x)))
