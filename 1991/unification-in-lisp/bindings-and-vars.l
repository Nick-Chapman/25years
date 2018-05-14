
(in-package "USER")

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   BINDINGS
;;   - Print with a dash notation
;;

(defstruct (binding (:print-function bindingprinter)) var val)

(defun bindingprinter (binding stream level)
  (declare (ignore stream))
  (declare (ignore level))
  (write (binding-var binding) :stream stream)
  (write '\- :stream stream)
  (write (binding-val binding) :stream stream))


(defun get-binding (var env)
  (if (null env)
      'no-binding
    (if (equalp var (binding-var (car env)))
	(car env)
      (get-binding var (cdr env)))))


(defun extend (var val env)
  (cons (make-Binding :var var :val val) env))


(defun lookup (var env)
  (let ((b (get-binding var env)))
    (if (eq b 'no-binding)
	'no-binding
      (binding-val b))))


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;;   VARIABLES
;;   - Read and Print using question marks.
;;

(defstruct (Var (:print-function VarPrinter)) v)

(defun VarPrinter (Var Stream Level)
  (declare (ignore Stream))
  (declare (ignore Level))
  (write '\? :stream Stream)
  (write (Var-v Var) :stream Stream))

(defun VarReader (Stream Char)
  (declare (ignore Char))
  (make-Var :v (read Stream)))

(set-macro-character #\? #'VarReader)
